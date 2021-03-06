/*
 *  yosys -- Yosys Open SYnthesis Suite
 *
 *  Copyright (C) 2012  Clifford Wolf <clifford@clifford.at>
 *  
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "kernel/register.h"
#include "kernel/sigtools.h"
#include "kernel/log.h"
#include "kernel/celltypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <set>

USING_YOSYS_NAMESPACE
PRIVATE_NAMESPACE_BEGIN

using RTLIL::id2cstr;

CellTypes ct, ct_reg, ct_all;
int count_rm_cells, count_rm_wires;

void rmunused_module_cells(RTLIL::Module *module, bool verbose)
{
	SigMap sigmap(module);
	std::set<RTLIL::Cell*, RTLIL::sort_by_name_id<RTLIL::Cell>> queue, unused;

	SigSet<RTLIL::Cell*> wire2driver;
	for (auto &it : module->cells_) {
		RTLIL::Cell *cell = it.second;
		for (auto &it2 : cell->connections()) {
			if (!ct.cell_input(cell->type, it2.first))
				wire2driver.insert(sigmap(it2.second), cell);
		}
		if (cell->type == "$memwr" || cell->type == "$assert" || cell->has_keep_attr())
			queue.insert(cell);
		unused.insert(cell);
	}

	for (auto &it : module->wires_) {
		RTLIL::Wire *wire = it.second;
		if (wire->port_output || wire->get_bool_attribute("\\keep")) {
			std::set<RTLIL::Cell*> cell_list;
			wire2driver.find(sigmap(wire), cell_list);
			for (auto cell : cell_list)
				queue.insert(cell);
		}
	}

	while (!queue.empty())
	{
		std::set<RTLIL::Cell*, RTLIL::sort_by_name_id<RTLIL::Cell>> new_queue;
		for (auto cell : queue)
			unused.erase(cell);
		for (auto cell : queue) {
			for (auto &it : cell->connections()) {
				if (!ct.cell_output(cell->type, it.first)) {
					std::set<RTLIL::Cell*> cell_list;
					wire2driver.find(sigmap(it.second), cell_list);
					for (auto c : cell_list) {
						if (unused.count(c))
							new_queue.insert(c);
					}
				}
			}
		}
		queue.swap(new_queue);
	}

	for (auto cell : unused) {
		if (verbose)
			log("  removing unused `%s' cell `%s'.\n", cell->type.c_str(), cell->name.c_str());
		module->design->scratchpad_set_bool("opt.did_something", true);
		module->remove(cell);
		count_rm_cells++;
	}
}

int count_nontrivial_wire_attrs(RTLIL::Wire *w)
{
	int count = w->attributes.size();
	count -= w->attributes.count("\\src");
	count -= w->attributes.count("\\unused_bits");
	return count;
}

bool compare_signals(RTLIL::SigBit &s1, RTLIL::SigBit &s2, SigPool &regs, SigPool &conns, std::set<RTLIL::Wire*> &direct_wires)
{
	RTLIL::Wire *w1 = s1.wire;
	RTLIL::Wire *w2 = s2.wire;

	if (w1 == NULL || w2 == NULL)
		return w2 == NULL;

	if (w1->port_input != w2->port_input)
		return w2->port_input;

	if (w1->name[0] == '\\' && w2->name[0] == '\\') {
		if (regs.check_any(s1) != regs.check_any(s2))
			return regs.check_any(s2);
		if (direct_wires.count(w1) != direct_wires.count(w2))
			return direct_wires.count(w2) != 0;
		if (conns.check_any(s1) != conns.check_any(s2))
			return conns.check_any(s2);
	}

	if (w1->port_output != w2->port_output)
		return w2->port_output;

	if (w1->name[0] != w2->name[0])
		return w2->name[0] == '\\';

	int attrs1 = count_nontrivial_wire_attrs(w1);
	int attrs2 = count_nontrivial_wire_attrs(w2);

	if (attrs1 != attrs2)
		return attrs2 > attrs1;

	return w2->name < w1->name;
}

bool check_public_name(RTLIL::IdString id)
{
	const std::string &id_str = id.str();
	if (id_str[0] == '$')
		return false;
	if (id_str.substr(0, 2) == "\\_" && (id_str[id_str.size()-1] == '_' || id_str.find("_[") != std::string::npos))
		return false;
	if (id_str.find(".$") != std::string::npos)
		return false;
	return true;
}

void rmunused_module_signals(RTLIL::Module *module, bool purge_mode, bool verbose)
{
	SigPool register_signals;
	SigPool connected_signals;

	if (!purge_mode)
		for (auto &it : module->cells_) {
			RTLIL::Cell *cell = it.second;
			if (ct_reg.cell_known(cell->type))
				for (auto &it2 : cell->connections())
					if (ct_reg.cell_output(cell->type, it2.first))
						register_signals.add(it2.second);
			for (auto &it2 : cell->connections())
				connected_signals.add(it2.second);
		}
	
	SigMap assign_map(module);
	std::set<RTLIL::SigSpec> direct_sigs;
	std::set<RTLIL::Wire*> direct_wires;
	for (auto &it : module->cells_) {
		RTLIL::Cell *cell = it.second;
		if (ct_all.cell_known(cell->type))
			for (auto &it2 : cell->connections())
				if (ct_all.cell_output(cell->type, it2.first))
					direct_sigs.insert(assign_map(it2.second));
	}
	for (auto &it : module->wires_) {
		if (direct_sigs.count(assign_map(it.second)) || it.second->port_input)
			direct_wires.insert(it.second);
	}

	for (auto &it : module->wires_) {
		RTLIL::Wire *wire = it.second;
		for (int i = 0; i < wire->width; i++) {
			RTLIL::SigBit s1 = RTLIL::SigBit(wire, i), s2 = assign_map(s1);
			if (!compare_signals(s1, s2, register_signals, connected_signals, direct_wires))
				assign_map.add(s1);
		}
	}

	module->connections_.clear();

	SigPool used_signals;
	SigPool used_signals_nodrivers;
	for (auto &it : module->cells_) {
		RTLIL::Cell *cell = it.second;
		for (auto &it2 : cell->connections_) {
			assign_map.apply(it2.second);
			used_signals.add(it2.second);
			if (!ct.cell_output(cell->type, it2.first))
				used_signals_nodrivers.add(it2.second);
		}
	}
	for (auto &it : module->wires_) {
		RTLIL::Wire *wire = it.second;
		if (wire->port_id > 0) {
			RTLIL::SigSpec sig = RTLIL::SigSpec(wire);
			assign_map.apply(sig);
			used_signals.add(sig);
			if (!wire->port_input)
				used_signals_nodrivers.add(sig);
		}
		if (wire->get_bool_attribute("\\keep")) {
			RTLIL::SigSpec sig = RTLIL::SigSpec(wire);
			assign_map.apply(sig);
			used_signals.add(sig);
		}
	}

	std::vector<RTLIL::Wire*> maybe_del_wires;
	for (auto wire : module->wires())
	{
		if ((!purge_mode && check_public_name(wire->name)) || wire->port_id != 0 || wire->get_bool_attribute("\\keep")) {
			RTLIL::SigSpec s1 = RTLIL::SigSpec(wire), s2 = s1;
			assign_map.apply(s2);
			if (!used_signals.check_any(s2) && wire->port_id == 0 && !wire->get_bool_attribute("\\keep")) {
				maybe_del_wires.push_back(wire);
			} else {
				log_assert(GetSize(s1) == GetSize(s2));
				RTLIL::SigSig new_conn;
				for (int i = 0; i < GetSize(s1); i++)
					if (s1[i] != s2[i]) {
						new_conn.first.append_bit(s1[i]);
						new_conn.second.append_bit(s2[i]);
					}
				if (new_conn.first.size() > 0) {
					used_signals.add(new_conn.first);
					used_signals.add(new_conn.second);
					module->connect(new_conn);
				}
			}
		} else {
			if (!used_signals.check_any(RTLIL::SigSpec(wire)))
				maybe_del_wires.push_back(wire);
		}

		RTLIL::SigSpec sig = assign_map(RTLIL::SigSpec(wire));
		if (!used_signals_nodrivers.check_any(sig)) {
			std::string unused_bits;
			for (int i = 0; i < GetSize(sig); i++) {
				if (sig[i].wire == NULL)
					continue;
				if (!used_signals_nodrivers.check(sig[i])) {
					if (!unused_bits.empty())
						unused_bits += " ";
					unused_bits += stringf("%d", i);
				}
			}
			if (unused_bits.empty() || wire->port_id != 0)
				wire->attributes.erase("\\unused_bits");
			else
				wire->attributes["\\unused_bits"] = RTLIL::Const(unused_bits);
		} else {
			wire->attributes.erase("\\unused_bits");
		}
	}


	std::set<RTLIL::Wire*> del_wires;

	int del_wires_count = 0;
	for (auto wire : maybe_del_wires)
		if (!used_signals.check_any(RTLIL::SigSpec(wire))) {
			if (check_public_name(wire->name) && verbose) {
				log("  removing unused non-port wire %s.\n", wire->name.c_str());
				del_wires_count++;
			}
			del_wires.insert(wire);
		}

	module->remove(del_wires);
	count_rm_wires += del_wires.size();;

	if (del_wires_count > 0)
		log("  removed %d unused temporary wires.\n", del_wires_count);
}

void rmunused_module(RTLIL::Module *module, bool purge_mode, bool verbose)
{
	if (verbose)
		log("Finding unused cells or wires in module %s..\n", module->name.c_str());

	std::vector<RTLIL::Cell*> delcells;
	for (auto cell : module->cells())
		if (cell->type.in("$pos", "$_BUF_")) {
			bool is_signed = cell->type == "$pos" && cell->getParam("\\A_SIGNED").as_bool();
			RTLIL::SigSpec a = cell->getPort("\\A");
			RTLIL::SigSpec y = cell->getPort("\\Y");
			a.extend_u0(GetSize(y), is_signed);
			module->connect(y, a);
			delcells.push_back(cell);
		}
	for (auto cell : delcells)
		module->remove(cell);

	rmunused_module_cells(module, verbose);
	rmunused_module_signals(module, purge_mode, verbose);
}

struct OptCleanPass : public Pass {
	OptCleanPass() : Pass("opt_clean", "remove unused cells and wires") { }
	virtual void help()
	{
		//   |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
		log("\n");
		log("    opt_clean [options] [selection]\n");
		log("\n");
		log("This pass identifies wires and cells that are unused and removes them. Other\n");
		log("passes often remove cells but leave the wires in the design or reconnect the\n");
		log("wires but leave the old cells in the design. This pass can be used to clean up\n");
		log("after the passes that do the actual work.\n");
		log("\n");
		log("This pass only operates on completely selected modules without processes.\n");
		log("\n");
		log("    -purge\n");
		log("        also remove internal nets if they have a public name\n");
		log("\n");
	}
	virtual void execute(std::vector<std::string> args, RTLIL::Design *design)
	{
		bool purge_mode = false;

		log_header("Executing OPT_CLEAN pass (remove unused cells and wires).\n");
		log_push();

		size_t argidx;
		for (argidx = 1; argidx < args.size(); argidx++) {
			if (args[argidx] == "-purge") {
				purge_mode = true;
				continue;
			}
			break;
		}
		extra_args(args, argidx, design);

		ct.setup_internals();
		ct.setup_internals_mem();
		ct.setup_stdcells();
		ct.setup_stdcells_mem();

		ct_reg.setup_internals_mem();
		ct_reg.setup_stdcells_mem();

		for (auto &mod_it : design->modules_) {
			if (!design->selected_whole_module(mod_it.first)) {
				if (design->selected(mod_it.second))
					log("Skipping module %s as it is only partially selected.\n", id2cstr(mod_it.second->name));
				continue;
			}
			if (mod_it.second->processes.size() > 0) {
				log("Skipping module %s as it contains processes.\n", mod_it.second->name.c_str());
			} else {
				rmunused_module(mod_it.second, purge_mode, true);
			}
		}

		ct.clear();
		ct_reg.clear();
		log_pop();
	}
} OptCleanPass;
 
struct CleanPass : public Pass {
	CleanPass() : Pass("clean", "remove unused cells and wires") { }
	virtual void help()
	{
		//   |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
		log("\n");
		log("    clean [options] [selection]\n");
		log("\n");
		log("This is identical to 'opt_clean', but less verbose.\n");
		log("\n");
		log("When commands are separated using the ';;' token, this command will be executed\n");
		log("between the commands.\n");
		log("\n");
		log("When commands are separated using the ';;;' token, this command will be executed\n");
		log("in -purge mode between the commands.\n");
		log("\n");
	}
	virtual void execute(std::vector<std::string> args, RTLIL::Design *design)
	{
		bool purge_mode = false;

		size_t argidx;
		for (argidx = 1; argidx < args.size(); argidx++) {
			if (args[argidx] == "-purge") {
				purge_mode = true;
				continue;
			}
			break;
		}
		if (argidx < args.size())
			extra_args(args, argidx, design);

		ct.setup_internals();
		ct.setup_internals_mem();
		ct.setup_stdcells();
		ct.setup_stdcells_mem();

		ct_reg.setup_internals_mem();
		ct_reg.setup_stdcells_mem();

		ct_all.setup(design);

		count_rm_cells = 0;
		count_rm_wires = 0;

		for (auto &mod_it : design->modules_) {
			if (design->selected_whole_module(mod_it.first) && mod_it.second->processes.size() == 0)
				do {
					design->scratchpad_unset("opt.did_something");
					rmunused_module(mod_it.second, purge_mode, false);
				} while (design->scratchpad_get_bool("opt.did_something"));
		}

		if (count_rm_cells > 0 || count_rm_wires > 0)
			log("Removed %d unused cells and %d unused wires.\n", count_rm_cells, count_rm_wires);

		ct.clear();
		ct_reg.clear();
		ct_all.clear();
	}
} CleanPass;
 
PRIVATE_NAMESPACE_END
