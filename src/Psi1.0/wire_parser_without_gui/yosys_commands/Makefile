
all: test0.log

my_cmd.so: hierarchy_cmd.cc
	/home/lunk/yosys/yosys-yosys-0.4/yosys-config --exec --cxx --cxxflags --ldflags -o hierarchy_cmd.so -shared hierarchy_cmd.cc --ldlibs

test0.log: my_cmd.so
	/home/lunk/yosys/yosys-yosys-0.4/yosys -Ql test0.log_new -m ./my_cmd.so -p 'my_cmd foo bar' generateStoreIndexes.v
	mv test0.log_new test0.log

#test1.log: my_cmd.so
	#/home/lunk/yosys/yosys-yosys-0.4/yosys -Ql test1.log_new -m ./my_cmd.so -p 'clean; test1; dump' #generateStoreIndexes.v
	#mv test1.log_new test1.log
#
#test2.log: my_cmd.so
#	/home/lunk/yosys/yosys-yosys-0.4/yosys -Ql test2.log_new -m ./my_cmd.so -p 'test2' sigmap_test.v
#	mv test2.log_new test2.log
clean: 
	rm -rf ./*.so
	echo Clean done
