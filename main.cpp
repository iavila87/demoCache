#include "cache.h"
#include <fstream>

class Student{
    int id;
    int value;
    const char * data;
public:
    static const string class_name; // Cada clase tiene un nombre static llamado : class_name
    Student (int _key , int _value , const char * _data ): id( _key), value ( _value ), data ( _data ) {}

    void print(){
        cout <<" Student Object : " <<id <<", " <<value <<", " <<data<< endl;
    };
    ~ Student () {};

};

const string Student :: class_name = " StudentClass ";


int main(){

    fstream ofs;
    ofs.open("datos.bin", ios:: binary | ios::out | ios::trunc);

    CacheManager <Student> my_cache(3);

    my_cache.insert_c("0", Student(0, 22, " student1 "));
    my_cache.insert_c("1", Student(1, 23, " student2 "));
    my_cache.insert_c("2", Student(2, 25, " student3 "));
    my_cache.insert_c("3", Student(3, 29, " student4 "));
    my_cache.show_cache();

    cout<<"----------------------UPDATE-----------------------"<<endl;
    my_cache.insert_c("2", Student(22 ,222 ,"EZE"));
    my_cache.show_cache();

    cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    Student return_obj = my_cache.get("0");//not in cache but in filesystem
    cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    return_obj.print();
    my_cache.show_cache();

    my_cache.insert_c("2", Student(2 ,25,"EZE"));
    my_cache.insert_c("9", Student(1,5," Nine "));
    my_cache.insert_c("9", Student(1,5," Nine "));
    my_cache.insert_c("9", Student(1,5," Nine "));
    my_cache.insert_c("9", Student(1,5," Nine "));
    my_cache.show_cache();

    cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    cout<<"<<<<<<<<<<< Trabajo Practico - Cache Manager <<<<<<<<<<<<"<<endl;
    cout<<"<<<<<<<< Asignatura : Diseno orientado a objetos <<<<<<<<"<<endl;
    cout<<"<<<<<<<<< Avila, Ivan E. - Torrazza, Virginia F. <<<<<<<<"<<endl;
    cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    return 0;
}
