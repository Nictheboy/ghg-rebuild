#ifndef single_source
#include "ghg.h"
#endif

void do_test(){
    using namespace std;
	cout<<"Testing file system...";
    if (file(file()).read_string()==""
		&& file().add_string("abcd").
				read_string()==file(file("ab")).
				add_string("cd").read_string()
    ){
		cout<<"ok\n";
	}else{
		cout<<"bad\n";
		exit(-1);
	}
}

