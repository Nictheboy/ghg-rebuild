/* Filename: file_system.hpp
 * Author: Nictheboy
 * Description: 
 *     This file provides two classes: file and
 *     dir, which is used in GHG's file system.
 * Modified by: Nictheboy
 * Modified date: 2021.01.30
 */

#ifndef file_system
#define file_system

#include <iostream>
#include <string>

//Empty file is allowed
class file{
public:
	struct fileImpl{
		virtual ~fileImpl(){};
		virtual void open() = 0;
		virtual void close() = 0;
		virtual bool read_to(void*ptr,
						size_t buff) = 0;
		virtual size_t get_size() = 0;
		virtual void rewrite(const void*ptr,
						size_t size) = 0;
		virtual void save() = 0;
		virtual void add(const void*ptr,
						size_t size){
		    size_t size0 = get_size();
			if (!size)return;
			if (!size0){
				rewrite(ptr,size);
				return;
			}
			void * buff = malloc(size0+size);
			if (!buff){
					std::cerr<<"failed to malloc"
							<<std::endl;
					exit(-1);
			}
			read_to(buff,size0);
			memcpy((char*)buff+size0,ptr,size);
			rewrite(buff,size0+size);
			free(buff);
		}
    };

    class memoryFileImpl:public fileImpl{
    private:
		char * data;
		size_t size;
		//When a file is empty,
		//size=0 and data=nullptr
    public:
		virtual void open(){}//do nothing
        virtual void close(){}//do nothing
        virtual bool read_to(void*ptr,
                        size_t buff){
		    if (!size){
				return true;
			}
			else if (buff>=size){
				memcpy(ptr,data,size);
				return true;
			}else{
				return false;
			}
		}
		virtual size_t get_size(){
		    return size;
		}
		virtual void rewrite(const void*ptr,                              size_t size){
		    if (this->size){free(data);}
			this->size = size;
			if (!size){
				data = nullptr;
				this->size = 0;
				return;
			}
		    data = (char*)malloc(size);
			if (!data){
				std::cerr<<"failed to malloc"
                            <<std::endl;
                    exit(-1);
            }
			memcpy(data,ptr,size);
		}
        virtual void save(){}//do nothing
		memoryFileImpl(const void*ptr,
						size_t size){
		    data = nullptr;
			this->size = 0;
			rewrite(ptr,size);
		}
		~memoryFileImpl(){
		    if(size) free(data);
			//When a file is freed,
			//the data will be freed
		}
	};
private:
	fileImpl * impl;
	bool opened;
public:
    file& open(){
		if (opened){
		    std::cerr
				<<"file is double opened"
				<<std::endl;
			return *(this);
		}
		opened = true;
		impl->open();
		return *(this);
	}

	file& close(){
		if (!opened){
				std::cerr
				    <<"file is not opened"
					<<std::endl;
				return *(this);
		}
		impl->close();
		return *(this);
	}

	bool if_opened()const{return opened;}

	const file& read_to(void*ptr,
					size_t buff)const{
		if (!opened){
				std::cerr
				    <<"file is not opened"
					<<std::endl;
				return *(this);
		}
		impl->read_to(ptr,buff);
		return *(this);
	}
    
	size_t get_size()const{
		if (!opened){
				std::cerr
				    <<"file is not opened"
					<<std::endl;
				return 0;
		}
		return impl->get_size();
	}
    
	file& rewrite(const void*ptr,size_t size){
		if (!opened){
				std::cerr
				    <<"file is not opened"
					<<std::endl;
				return *(this);
		}
		impl->rewrite(ptr,size);
		return *(this);
	}

    file& save(){
		if (!opened){
				std::cerr
				    <<"file is not opened"
					<<std::endl;
				return *(this);
		}
		impl->save();
		return *(this);
	}

	file& add(const void*ptr,size_t size){
		if (!opened){
				std::cerr
				    <<"file is not opened"
					<<std::endl;
				return *(this);
		}
		impl->add(ptr,size);
		return *(this);
	}

	void * read()const{
		//An empty file returns nullptr
		//When you read() a file, you should
		//free() the temp data after you use it.
		if (!opened){
				std::cerr
				    <<"file is not opened"
					<<std::endl;
				return nullptr;
		}
		size_t size = get_size();
		if (!size)return nullptr;
		void * buff = malloc(size);
		if (!buff){
		    std::cerr<<"failed to malloc"
                    <<std::endl;
		    exit(-1);
        }
		read_to(buff,size);
		return buff;
	}

	file& rewrite_string(std::string str){
		if (!opened){
				std::cerr
				    <<"file is not opened"
					<<std::endl;
				return *(this);
		}
		const char * c_str = str.c_str();
		rewrite(c_str,strlen(c_str) + 1);
		return *(this);
	}

	std::string read_string()const{
		if (!opened){
				std::cerr
				    <<"file is not opened"
					<<std::endl;
				return "";
		}
		if (!get_size())return "";
		char * c_str = (char*)read();
		*(c_str + get_size() - 1) = '\0';
		//Set the end of temp data as \0,
		//in order to prevent getting a string
		//that is longger than the file
		std::string str = std::string(c_str);
		free(c_str);
		return str;
	}
	
	file& add_string(std::string str){
		if (!opened){
				std::cerr
				    <<"file is not opened"
					<<std::endl;
				return *(this);
		}
		rewrite_string(read_string()+str);
		return *(this);
	}

    //By default, a memory-file will 
    //be initialized and opened
	file(const void*ptr,size_t size){
        opened = false;
		impl = new memoryFileImpl(ptr,size);
		open();
    }

	file(){
        opened = false;
		impl = new memoryFileImpl(nullptr,0);
		open();
	}

	file(std::string s){
        opened = false;
		impl = new memoryFileImpl(nullptr,0);
		open();
		rewrite_string(s);
	}

	file(const file & f){
		opened = false;
		void * buff = f.read();
		impl = new memoryFileImpl(buff,
						f.get_size());
		free(buff);
		open();

	}

	~file(){
		if (opened)impl->close();
		delete impl;
	}
};

#endif

