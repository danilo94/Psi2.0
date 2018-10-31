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
#include "kernel/celltypes.h"
#include "kernel/rtlil.h"
#include "kernel/log.h"

USING_YOSYS_NAMESPACE
PRIVATE_NAMESPACE_BEGIN

struct SplitnetsWorker
{
	std::map<RTLIL::Wire*, std::vector<RTLIL::SigBit>> splitmap;

	void append_wire(RTLIL::Module *module, RTLIL::Wire *wire, int offset, int width, std::string format)
	{
		std::string new_wire_name = wire->name.str();

		if (format.size() > 0)
			new_wire_name += format.substr(0, 1);

		if (width > 1) {
			new_wire_name += stringf("%d", offset+width-1);
			if (format.size() > 2)
				new_wire_name += format.substr(2, 1);
			else
				new_wire_name += ":";
		}

		new_wire_name += stringf("%d", offset);

		if (format.size() > 1)
			new_wire_name += format.substr(1, 1);

		RTLIL::Wire *new_wire = module->addWire(module->uniquify(new_wire_name), width);
		new_wire->port_id = wire->port_id;
		new_wire->port_input = wire->port_input;
		new_wire->port_output = wire->port_output;

		std::vector<RTLIL::SigBit> sigvec = RTLIL::SigSpec(new_wire).to_sigbit_vector();
		splitmap[wire].insert(splitmap[wire].end(), sigvec.begin(), sigvec.end());
	}

	void operator()(RTLIL::SigSpec &sig)
	{
		for (auto &bit : sig)
			if (splitmap.count(bit.wire) > 0)
				bit = splitmap.at(bit.wire).at(bit.offset);
	}
};

struct SplitnetsPass : public Pass {
	SplitnetsPass() : Pass("splitnets", "split up multi-bit nets") { }
	virtual void help()
	{
		//   |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
		log("\n");
		log("    splitnets [options] [selection]\n");
		log("\n");
		log("This command splits multi-bit nets into single-bit nets.\n");
		log("\n");
		log("    -format char1[char2[char3]]\n");
		log("        the first char is inserted between the net name and the bit index, the\n");
		log("        second char is appended to the netname. e.g. -format () creates net\n");
		log("        names like 'mysignal(42)'. the 3rd character is the range separation\n");
		log("        character when creating multi-bit wires. the default is '[]:'.\n");
		log("\n");
		log("    -ports\n");
		log("        also split module ports. per default only internal signals are split.\n");
		log("\n");
		log("    -driver\n");
		log("        don't blindly split nets in individual bits. instead look at the driver\n");
		log("        and split nets so that no driver drives only part of a net.\n");
		log("\n");
	}
	virtual void execute(std::vector<std::string> args, RTLIL::Design *design)
	{
		bool flag_ports = false;
		bool flag_driver = false;
		std::string format = "[]:";

		log_header("Executing SPLITNETS pass (splitting up multi-bit signals).\n");

		size_t argidx;
		for (argidx = 1; argidx < args.size(); argidx++)
		{
			if (args[argidx] == "-format" && argidx+1 < args.size()) {
				format = args[++argidx];
				continue;
			}
			if (args[argidx] == "-ports") {
				flag_ports = true;
				continue;
			}
			if (args[argidx] == "-driver") {
				flag_driver = true;
				continue;
			}
			break;
		}
		extra_args(args, argidx, design);

		for (auto &mod_it : design->modules_)
		{
			RTLIL::Module *module = mod_it.second;
			if (!design->selected(module))
				continue;

			SplitnetsWorker worker;

			if (flag_driver)
			{
				CellTypes ct(design);

				std::map<RTLIL::Wire*, std::set<int>> split_wires_at;

				for (auto &c : module->cells_)
				for (auto &p : c.second->connections())
				{
					if (!ct.cell_known(c.second->type))
						continue;
					if (!ct.cell_output(c.second->type, p.first))
						continue;

					RTLIL::SigSpec sig = p.second;
					for (auto &chunk : sig.chunks()) {
						if (chunk.wire == NULL)
							continue;
						if (chunk.wire->port_id == 0 || flag_ports) {
							if (chunk.offset != 0)
								split_wires_at[chunk.wire].insert(chunk.offset);
							if (chunk.offset + chunk.width < chunk.wire->width)
								split_wires_at[chunk.wire].insert(chunk.offset + chunk.width);
						}
					}
				}

				for (auto &it : split_wires_at) {
					int cursor = 0;
					for (int next_cursor : it.second) {
						worker.append_wire(module, it.first, cursor, next_cursor - cursor, format);
						cursor = next_cursor;
					}
					worker.append_wire(module, it.first, cursor, it.first->width - cursor, format);
				}
			}
			else
			{
				for (auto &w : module->wires_) {
					RTLIL::Wire *wire = w.second;
					if (wire->width > 1 && (wire->port_id == 0 || flag_ports) && design->selected(module, w.second))
						worker.splitmap[wire] = std::vector<RTLIL::SigBit>();
				}

				for (auto &it : worker.splitmap)
					for (int i = 0; i < it.first->width; i++)
						worker.append_wire(module, it.first, i, 1, format);
			}

			module->rewrite_sigspecs(worker);

			std::set<RTLIL::Wire*> delete_wires;
			for (auto &it : worker.splitmap)
				delete_wires.insert(it.first);
			module->remove(delete_wires);

			module->fixup_ports();
		}
	}
} SplitnetsPass;
 
PRIVATE_NAMESPACE_END
