#include <iostream >
#include <fstream >
#include <map >
#include <utility >
#include <string >
#include <iterator>
#include <vector>
#include <string.h>

using namespace std;

/** Estructura utilizada para el alamacenamiento */
/** de datos en archivos.                        */
template <class T>
struct  files{
            char k[10];
            T obj;
            files() : k(""), obj (T (0, 0, "")){}
        };


template <class T>
class CacheManager{
    int         capacity;
    static int  ML_RU;
    map<string, pair<T, int>> cache_data; /// <Clave ,<Obj ,Indice de Uso >>.
    bool write_file(string ,T );
    bool read_file (string );
    bool update_file(string ,T );
public :
    CacheManager(int); /// Recibe la capacidad de almacenamiento de la cache.
    ~CacheManager();

    void insert_c(string key , T obj);
    T get(string key);
    void show_cache(void);
};

template <class T>
int CacheManager <T> :: ML_RU = 0;

template <class T> /// Constructor
CacheManager <T>:: CacheManager(int cap){
    capacity = cap;
}

template <class T> /// Destructor
CacheManager <T>::~ CacheManager(){}

/** Funcion que permite escribir en un archivo binario los datos */
/** de key y objeto recibidos                                    */
template <class T>
bool CacheManager <T>:: write_file (string key, T obj) {

    files <T> f1;
    strcpy(f1.k, key.c_str());
    f1.obj = obj;
    f1.obj.print();
    /// Apertura del archivo para su escritura.
    ofstream f("datos.bin", ios::binary | ios::app);
    if(f.is_open()){
        f.write((char *) &f1, sizeof(files <T>));
    }else{
        cout << "Error de apertura de archivo." << endl;
        return false;
    }
    f.close();
    cout<<"--- Archivo Actualizado ---"<<endl;
    return true;
}

/** Funcion que retorna el objeto ligado a la key que            */
/** recibe como argumento                                        */
template <class T>
T CacheManager <T>:: get(string key){
    typename map<string, pair<T, int>>::iterator it;
    ML_RU++;
    it = cache_data.find(key);
    /** Verifico que la clave se encuentre en la cache */
    if(it != cache_data.end()){
        it->second.second = ML_RU;
        return it->second.first;
    }else{
        /** Busco la key en el archivo ya que la misma no se encuentra en cache */
        ML_RU--;
        if(read_file(key)){
            it = cache_data.find(key);
            return it->second.first;
        }else{
            cout<<"Key solicitada es inexistente"<<endl;
            it = cache_data.begin();
            T temp (0,0,"");
            return temp;
        }
    }
}

/** Funcion que permite leer los datos en el archivo binario     */
/** y volcarlos a cache                                          */
template <class T>
bool CacheManager <T>:: read_file (string key) {
    vector <files <T> > v;
    files<T> f2;
    ifstream fr( "datos.bin", ios::binary | ios::in);

    if(fr.is_open()){
        while(fr.read((char *) &f2, sizeof(files<T>))){
            v.push_back(f2);
        }
    }else{
        cout << "Error de apertura de archivo." << endl;
        return false;
    }
    fr.close();

    for(int i = 0; i<v.size();i++){
        if(key == v[i].k){
            insert_c(v[i].k, v[i].obj);
            return true;
        }
    }
    return false;
}

/** Funcion que actualiza los datos en el archivo binario        */
/**                                                              */
template <class T>
bool CacheManager <T>::update_file(string key,T obj){

    vector <files<T>> v;
    files<T> f3;

    ifstream fr("datos.bin", ios::binary);
    if(fr.is_open()){
        while(fr.read((char *) &f3, sizeof(files<T>))){
            v.push_back(f3);
        }
    }else{
        cout << "Error de apertura de archivo." << endl;
        return false;
    }
    fr.close();

    for(int i = 0; i<v.size();i++){
        if(key == v[i].k){
            v[i].obj = obj;
        }
    }

    ofstream f("datos.bin", ios::binary );
    if(f.is_open()){
        f.write( (char *) &v[0], v.size()*sizeof(files<T>) );
    }else{
        cout << "Error de apertura de archivo." << endl;
        return false;
    }
    f.close();
    cout<<"--- Archivo Actualizado ---"<<endl;
    return true;
}

/** Funcion que inserta datos tanto en la cache como             */
/** en el archivo binario                                        */
template <class T>
void CacheManager <T>:: insert_c(string key, T obj){

    typename map<string, pair<T, int>>::iterator it;
    ML_RU++;
    it = cache_data.find(key);
    /** Verifico que la clave se encuentre en la cache */
    if(it != cache_data.end()){
        /** Actualizo Cache */
        it->second = make_pair(obj,ML_RU);
        /** Actualizo Archivo */
        update_file(it->first,obj);
    }else{
    /** Si no esta en la cache corroboro que la misma posea capacidad
    de almacenamiento */
        if(cache_data.size()>=capacity){
            /** Se escribe en el archivo y en la cache por el metodo
            de sustitucion LRU (menos usado recientemente) */
            string auxK;
            int aux = it->second.second;
            for (it=cache_data.begin(); it!=cache_data.end(); ++it){
                if (aux > it->second.second){
                    aux = it->second.second;
                    auxK = it->first;
                }
            }
            cache_data.erase(auxK);
            cache_data.insert(pair<string,pair<T,int>>(key,make_pair(obj,ML_RU)));
            write_file(key,obj);
        }else{
            /** la cache cuenta con espacio disponible */
            cache_data.insert(pair<string,pair<T,int>>(key,make_pair(obj,ML_RU)));
            write_file(key,obj);
        }
    }
}

/** Funcion para mostrar el contenido de la cache                */
template <class T>
void CacheManager <T>:: show_cache(void){
    typename map<string, pair<T, int>> ::iterator it;
    cout<<"Contenido de la Cache: "<<endl;
    for(auto it = cache_data.begin();it != cache_data.end(); ++it){
        cout <<"key: " << it->first << endl;
        it->second.first.print();
        cout<< "MRU/LRU: "<<it->second.second << endl;
    }
}
