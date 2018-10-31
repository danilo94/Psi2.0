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

#include "kernel/yosys.h"

#ifndef LOG_H
#define LOG_H

#include <time.h>

#ifndef _WIN32
#  include <sys/time.h>
#  include <sys/resource.h>
#endif

// from libs/sha1/sha1.h
class SHA1;

YOSYS_NAMESPACE_BEGIN

#define S__LINE__sub2(x) #x
#define S__LINE__sub1(x) S__LINE__sub2(x)
#define S__LINE__ S__LINE__sub1(__LINE__)

struct log_cmd_error_exception { };

extern std::vector<FILE*> log_files;
extern std::vector<std::ostream*> log_streams;
extern FILE *log_errfile;
extern SHA1 *log_hasher;

extern bool log_time;
extern bool log_cmd_error_throw;
extern int log_verbose_level;

void logv(const char *format, va_list ap);
void logv_header(const char *format, va_list ap);

void logv_error(const char *format, va_list ap) __attribute__((noreturn));
void log(const char *format, ...)  __attribute__((format(printf, 1, 2)));
void log_header(const char *format, ...) __attribute__((format(printf, 1, 2)));
_NORETURN_ void log_error(const char *format, ...) __attribute__((format(printf, 1, 2))) __attribute__((noreturn));
_NORETURN_ void log_cmd_error(const char *format, ...) __attribute__((format(printf, 1, 2))) __attribute__((noreturn));

void log_spacer();
void log_push();
void log_pop();

void log_reset_stack();
void log_flush();

const char *log_signal(const RTLIL::SigSpec &sig, bool autoint = true);
const char *log_id(RTLIL::IdString id);

template<typename T> static inline const char *log_id(T *obj) {
	return log_id(obj->name);
}

void log_cell(RTLIL::Cell *cell, std::string indent = "");

static inline void log_assert_worker(bool cond, const char *expr, const char *file, int line) {
	if (!cond) log_error("Assert `%s' failed in %s:%d.\n", expr, file, line);
}

#define log_abort() YOSYS_NAMESPACE_PREFIX log_error("Abort in %s:%d.\n", __FILE__, __LINE__)
#define log_assert(_assert_expr_) YOSYS_NAMESPACE_PREFIX log_assert_worker(_assert_expr_, #_assert_expr_, __FILE__, __LINE__)
#define log_ping() YOSYS_NAMESPACE_PREFIX log("-- %s:%d %s --\n", __FILE__, __LINE__, __PRETTY_FUNCTION__)


// ---------------------------------------------------
// This is the magic behind the code coverage counters
// ---------------------------------------------------

#ifdef YOSYS_ENABLE_COVER

#define cover(_id) do { \
    static CoverData __d __attribute__((section("yosys_cover_list"), aligned(1), used)) = { __FILE__, __FUNCTION__, _id, __LINE__, 0 }; \
    __d.counter++; \
} while (0)

struct CoverData {
	const char *file, *func, *id;
	int line, counter;
} __attribute__ ((packed));

// this two symbols are created by the linker for the "yosys_cover_list" ELF section
extern "C" struct CoverData __start_yosys_cover_list[];
extern "C" struct CoverData __stop_yosys_cover_list[];

extern std::map<std::string, std::pair<std::string, int>> extra_coverage_data;

void cover_extra(std::string parent, std::string id, bool increment = true);
std::map<std::string, std::pair<std::string, int>> get_coverage_data();

#define cover_list(_id, ...) do { cover(_id); \
	std::string r = cover_list_worker(_id, __VA_ARGS__); \
	log_assert(r.empty()); \
} while (0)

static inline std::string cover_list_worker(std::string, std::string last) {
	return last;
}

template<typename... T>
std::string cover_list_worker(std::string prefix, std::string first, T... rest) {
	std::string selected = cover_list_worker(prefix, rest...);
	cover_extra(prefix, prefix + "." + first, first == selected);
	return first == selected ? "" : selected;
}

#else
#  define cover(...) do { } while (0)
#  define cover_list(...) do { } while (0)
#endif


// ------------------------------------------------------------
// everything below this line are utilities for troubleshooting
// ------------------------------------------------------------

// simple timer for performance measurements
// toggle the '#if 1' to get a baseline for the perormance penalty added by the measurement
struct PerformanceTimer
{
#if 1
	int64_t total_ns;

	PerformanceTimer() {
		total_ns = 0;
	}

	static int64_t query() {
#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
		struct timespec ts;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
		return int64_t(ts.tv_sec)*1000000000 + ts.tv_nsec;
#elif defined(RUSAGE_SELF)
		struct rusage rusage;
		int64_t t;
		if (getrusage(RUSAGE_SELF, &rusage) == -1) {
			log_cmd_error("getrusage failed!\n");
			log_abort();
		}
		t = 1000000000ULL * (int64_t) rusage.ru_utime.tv_sec + (int64_t) rusage.ru_utime.tv_usec * 1000ULL;
		t += 1000000000ULL * (int64_t) rusage.ru_stime.tv_sec + (int64_t) rusage.ru_stime.tv_usec * 1000ULL;
		return t;
#elif _WIN32
		return 0;
#else
	#error Dont know how to measure per-process CPU time. Need alternative method (times()/clocks()/gettimeofday()?).
#endif
	}

	void reset() {
		total_ns = 0;
	}

	void begin() {
		total_ns -= query();
	}

	void end() {
		total_ns += query();
	}

	float sec() const {
		return total_ns * 1e-9f;
	}
#else
	static int64_t query() { return 0; }
	void reset() { }
	void begin() { }
	void end() { }
	float sec() const { return 0; }
#endif
};

// simple API for quickly dumping values when debugging

static inline void log_dump_val_worker(short v) { log("%d", v); }
static inline void log_dump_val_worker(unsigned short v) { log("%u", v); }
static inline void log_dump_val_worker(int v) { log("%d", v); }
static inline void log_dump_val_worker(unsigned int v) { log("%u", v); }
static inline void log_dump_val_worker(long int v) { log("%ld", v); }
static inline void log_dump_val_worker(unsigned long int v) { log("%lu", v); }
#ifndef _WIN32
static inline void log_dump_val_worker(long long int v) { log("%lld", v); }
static inline void log_dump_val_worker(unsigned long long int v) { log("%lld", v); }
#endif
static inline void log_dump_val_worker(char c) { log(c >= 32 && c < 127 ? "'%c'" : "'\\x%02x'", c); }
static inline void log_dump_val_worker(unsigned char c) { log(c >= 32 && c < 127 ? "'%c'" : "'\\x%02x'", c); }
static inline void log_dump_val_worker(bool v) { log("%s", v ? "true" : "false"); }
static inline void log_dump_val_worker(double v) { log("%f", v); }
static inline void log_dump_val_worker(char *v) { log("%s", v); }
static inline void log_dump_val_worker(const char *v) { log("%s", v); }
static inline void log_dump_val_worker(std::string v) { log("%s", v.c_str()); }
static inline void log_dump_val_worker(PerformanceTimer p) { log("%f seconds", p.sec()); }
static inline void log_dump_args_worker(const char *p) { log_assert(*p == 0); }
void log_dump_val_worker(RTLIL::SigSpec v);

template<typename T>
static inline void log_dump_val_worker(T *ptr) { log("%p", ptr); }

template<typename T, typename ... Args>
void log_dump_args_worker(const char *p, T first, Args ... args)
{
	int next_p_state = 0;
	const char *next_p = p;
	while (*next_p && (next_p_state != 0 || *next_p != ',')) {
		if (*next_p == '"')
			do {
				next_p++;
				while (*next_p == '\\' && *(next_p + 1))
					next_p += 2;
			} while (*next_p && *next_p != '"');
		if (*next_p == '\'') {
			next_p++;
			if (*next_p == '\\')
				next_p++;
			if (*next_p)
				next_p++;
		}
		if (*next_p == '(' || *next_p == '[' || *next_p == '{')
			next_p_state++;
		if ((*next_p == ')' || *next_p == ']' || *next_p == '}') && next_p_state > 0)
			next_p_state--;
		next_p++;
	}
	log("\n\t%.*s => ", int(next_p - p), p);
	if (*next_p == ',')
		next_p++;
	while (*next_p == ' ' || *next_p == '\t' || *next_p == '\r' || *next_p == '\n')
		next_p++;
	log_dump_val_worker(first);
	log_dump_args_worker(next_p, args ...);
}

#define log_dump(...) do { \
	log("DEBUG DUMP IN %s AT %s:%d:", __PRETTY_FUNCTION__, __FILE__, __LINE__); \
	log_dump_args_worker(#__VA_ARGS__, __VA_ARGS__); \
	log("\n"); \
} while (0)

YOSYS_NAMESPACE_END

#endif