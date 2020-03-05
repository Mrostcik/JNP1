#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>
#include <string>
#include "poset.h"

using namespace std;

#define MP make_pair
#define getId first
#define counter second

using UL = long unsigned;

// Typ kontenera, ktory konwertuje napisy w liczby i pamieta ile razy dany napis wystepuje
// w zbiorze posetow.
#define stringToInt map<string, pair<UL, UL>>

// Typ reprezentujacy graf, dla kazdego wierzcholka trzymam set wierzcholkow,
// z ktorych moge do niego dojsc.
#define graph map<UL, set<UL>>

// Typ reprezentujacy zbior posetow, kluczami mapy sa numery posetow, a
// wartosciami para dwoch grafow skierowanych, jeden reprezentujacy poset,
// gdzie krawedz z A do B oznacza A < B, a drugi graf jest grafem odwrotnym do pierwszego,
// czyli dla kazdej krawedzi z A do B w grafie pierwszym, graf drugi zawiera krawedz
// z B do A.
#define posetsCollection map<UL, pair<graph, graph>>

//Flaga wylaczajaca tryb debug - wypisywanie na wyjscie diagnostyczne
#ifdef NDEBUG
	bool debug(){
		return false;
	}
#else
	bool debug(){
		//Upewniamy sie, ze strumienie sa zainicjalizowane
		ios_base::Init();
		return true;
	}
#endif

//Przydatne zmienne globalne
namespace {

	posetsCollection &posets(){
		static posetsCollection posets;
		return posets;
	}

	set <UL> &posetsCreated(){
		static set <UL> posetsCreated;
		return posetsCreated;
	}

	UL &totalPosets(){
		static UL totalPosets = 0;
		return totalPosets;
	}

	size_t &totalStrings(){
		static size_t totalStrings = 0;
		return totalStrings;
	}

	stringToInt &converter(){
		static stringToInt converter;
		return converter;
	}

	map <UL, string> &rev_converter(){
		static map <UL, string> rev_converter;
		return rev_converter;
	}

}

//Wypisuje podany napis na wyjscie diagnostyczne
void printToCerr(string s){
	if(debug()){
		cerr<<s<<endl;
	}
}

//Zwraca informacje, ktore z value1 i value2 sa NULLami
//Zmienia NULLA na napis "NULL", aby ulatwic wypisywanie na stderr
//3 - oba sa NULLami, 2 - value2 jest NULLem, 1 - value1 jest NULLem
int checkTwoValues(const char** value1, const char** value2){
	if(*value1 == NULL && *value2 == NULL){
		*value1 = "NULL";
		*value2 = "NULL";
		return 3;
	}
	else if(*value2 == NULL){
		*value2 = "NULL";
		return 2;
	}
	else if(*value1 == NULL){
		*value1 = "NULL";
		return 1;
	}

	return 0;
}

// Jeżeli jakiś napis nie występuje w żadnym posecie,
// to jest usuwany z mapy.
static void convert_erase(const string& s, UL s_id) {
    if (!--converter()[s].second) {
        converter().erase(s);
        rev_converter().erase(s_id);
    }
}

// Przypisuje stringowi indentyfikator: liczbe calkowita nieujemna, kazda liczba 
// przypisana jest do co najwyzej jednego stringa.
UL addStringId(char const *s) {
    if (converter().count(s)) {
        converter()[s].counter++;
        return converter()[s].getId;
    }
    totalStrings()++;
    converter()[s] = MP(totalStrings() , 1);

    return totalStrings();
}

// Zwraca indentyfikator napisu, lub 0 jeżeli napis nie ma przypisanego indentyfikatora.
UL getStringId(char const* s) {
    if (converter().count(s))
        return converter()[s].getId;

    return 0;
}

// Zmniejsza ilość wystapien danego napisu w konwerterze, jezeli
// ilosc wystapien osiagnie 0, to usuwa ten napis z konwertera.
void decreseStringId(char const *s) {
    converter()[s].counter--;

    if (!converter()[s].counter)
        converter().erase(s);
}

// Sprawdza czy istnieje sciezka pomiedzy v1 i v2 w grafie skierowanym G.
bool existsPath(graph G, UL v1, UL v2) {
    queue<UL> que;

    unordered_set<UL> visited;
    que.push(v1);
    visited.insert(v1);

    while (!que.empty()) {
        UL v = que.front();
        que.pop();

        for (UL x : G[v]) {
            if (x == v2)
                return true;
            if (!visited.count(x)) {
                que.push(x);
                visited.insert(x);
            }
        }
    }

    return false;
}


namespace jnp1{
	// Tworzy nowy poset o numerze, który jest liczbą wczesniej stworzonycn posetow.
	// Zwracany jest numer stworzoneo poseta.
	UL poset_new() {
		posetsCreated().insert(totalPosets()++);

		printToCerr("poset_new()");
		printToCerr("poset_new(): poset " + to_string(totalPosets() - 1) + " created");
		return totalPosets() - 1;
	}

	// Usuwa poset o numerze id, nie robi nic jezeli taki poset nie istnieje.
	void poset_delete(UL id) {
		printToCerr("poset_delete(" + to_string(id) + ")");

		if (posetsCreated().count(id)) {

			graph &G = posets()[id].first;
			for (pair <UL, set<UL>> pairs: G)
				convert_erase(rev_converter()[pairs.first], pairs.first);

			posetsCreated().erase(id);
			posets()[id].first.clear();
			posets()[id].second.clear();
			posets().erase(posets().find(id));

			printToCerr("poset_delete: poset " + to_string(id) + " deleted");
			return;
		}

		printToCerr("poset_delete: poset " + to_string(id) + " does not exist");
	}

	// Zwraca ilosc elementow w posecie o numerze id.
	size_t poset_size(UL id) {
		printToCerr("poset_size(" + to_string(id) + ")");

		if(!posetsCreated().count(id)){
			printToCerr("poset_size: poset " + to_string(id) + " does not exist");
			return 0;
		}

		printToCerr("poset_size: poset " + to_string(id) + " contains " + to_string(posets()[id].first.size()) + " element(s)");
		return posets()[id].first.size();
	}

	// Dodaje wartosc do poseta o podanym numerze.
    bool poset_insert(UL id, char const *value) {
        bool broken = 0;
        if(value == NULL){
            value = "NULL";
            broken = 1;
        }

        printToCerr("poset_insert(" + to_string(id) + ", \"" + value + "\")");

        if (!posetsCreated().count(id)){
            printToCerr("poset_insert: poset " + to_string(id) + " does not exist");
            return false;
        }

        if(broken){
            printToCerr("poset_insert: invalid value (NULL)");
            return false;
        }

        graph &G = posets()[id].first;
        graph &G_reversed = posets()[id].second;
        UL valueId = addStringId(value);

        if (G.count(valueId)) {
            decreseStringId(value);
            printToCerr("poset_insert: poset " + to_string(id) + ", element \"" + value + "\" already exists");
            return false;
        }


        G[valueId].insert(valueId);
        G_reversed[valueId].insert(valueId);

        printToCerr("poset_insert: poset " + to_string(id) + ", element \"" + value + "\" inserted");
        return true;
    }

    // Usuwa z poseta podana wartosc.
	bool poset_remove(UL id, char const *value) {
		bool broken = 0;
		if(value == NULL){
			value = "NULL";
			broken = 1;
		}

		printToCerr("poset_remove(" + to_string(id) + ", \"" + value + "\")");

		if (!posets().count(id)){
			printToCerr("poset_remove: poset " + to_string(id) + " does not exist");
			return false;
		}

		if(broken){
			printToCerr("poset_remove: invalid value (NULL)");
			return false;
		}

		UL valueId = getStringId(value);
		if (!posets()[id].first.count(valueId)){
			printToCerr("poset_remove: poset " + to_string(id) + ", element \"" + value + "\" does not exist");
			return false;
		}

		graph &G = posets()[id].first;
		graph &G_reversed = posets()[id].second;

		// Usuwam wierzcholek z setow sasiedztwa pozostalych wierzcholkow i
		// lacze kazde dwa wierzcholki A i B takie, że z A wychodzi krawedz do
		// usuwanego wierzcholka, a  do B wchodzi krawedz z usuwanego wierzcholka.
		for (UL val1 : G_reversed[valueId])
			G[val1].erase(valueId);
		for (UL val1 : G[valueId])
			G_reversed[val1].erase(valueId);

		for (UL val1 : G_reversed[valueId])
			for (UL val2: G[valueId]){
				if(val2 != valueId)
					G[val1].insert(val2);
			}

		for (UL val1 : G[valueId])
			for (UL val2 : G_reversed[valueId]){
				if(val2 != valueId)
					G_reversed[val1].insert(val2);
			}


		// Jezeli napis nie wystepuje w zadnym posecie to go usuwam
		--converter()[value].counter;
		if (!converter()[value].counter)
			converter().erase(value);

		G.erase(valueId);
		G_reversed.erase(valueId);

		printToCerr("poset_remove: poset " + to_string(id) + ", element \"" + value + "\" removed");
		return true;
	}

	// Dodaje relacje do posetu o zadanym numerze.
	bool poset_add(UL id, char const *value1, char const* value2){
		int broken = checkTwoValues(&value1, &value2);

		printToCerr("poset_add(" + to_string(id) + ", \"" + value1 + "\", \"" + value2 + "\")");

		if(!posets().count(id)){
			printToCerr("poset_add: poset " + to_string(id) + " does not exist");
			return false;
		}

		if(broken == 3){
			printToCerr("poset_add: invalid value1 (NULL)");
			printToCerr("poset_add: invalid value2 (NULL)");
			return false;
		}
		else if(broken == 2){
			printToCerr("poset_add: invalid value2 (NULL)");
			return false;
		}
		else if(broken == 1){
			printToCerr("poset_add: invalid value1 (NULL)");
			return false;
		}

		UL valueId1 = getStringId(value1),
				valueId2 = getStringId(value2);

		graph &G = posets()[id].first;
		graph &G_reversed = posets()[id].second;

	    if (!G.count(valueId1) || !G.count(valueId2)){
	    	printToCerr("poset_add: poset " + to_string(id) + ", element \"" + value1 + "\" or \"" +
	    			value2 + "\" does not exist");
	    	return false;
	    }

	    // Sprawdza czy dwa elementy nie sa juz w relacji.
		if (valueId1 == valueId2 || existsPath(G, valueId1, valueId2) || existsPath(G_reversed, valueId1, valueId2)){
			printToCerr("poset_add: poset " + to_string(id) + ", relation(\"" + value1 + "\", \"" +
					value2 + "\") cannot be added");
			return false;
		}

		G[valueId1].insert(valueId2);
		G_reversed[valueId2].insert(valueId1);

		printToCerr("poset_add: poset " + to_string(id) + ", relation(\"" + value1 + "\", \"" +
				value2 + "\") added");
		return true;
	}

    // Usuwa relecje z posetu o zadanym numerze.
	bool poset_del(UL id, char const* value1, char const* value2) {
		int broken = checkTwoValues(&value1, &value2);
		printToCerr("poset_del(" + to_string(id) + ", \"" + value1 + "\", \"" + value2 + "\")");

		if(!posets().count(id)){
			printToCerr("poset_del: poset " + to_string(id) + " does not exist");
			return false;
		}

		if(broken == 3){
			printToCerr("poset_del: invalid value1 (NULL)");
			printToCerr("poset_del: invalid value2 (NULL)");
			return false;
		}
		else if(broken == 2){
			printToCerr("poset_del: invalid value2 (NULL)");
			return false;
		}
		else if(broken == 1){
			printToCerr("poset_del: invalid value1 (NULL)");
			return false;
		}

		UL v1 = getStringId(value1);
		UL v2 = getStringId(value2);

		graph &G = posets()[id].first;
		graph &G_reversed = posets()[id].second;

	    if (!G.count(v1) || !G.count(v2)){
	    	printToCerr("poset_del: poset " + to_string(id) + ", element \"" + value1 + "\" or \"" +
	    			value2 + "\" does not exist");
	    	return false;
	    }

		if ((!v1 || !v2) || (v1 == v2) || !G[v1].count(v2) || !G_reversed[v2].count(v1)){
			printToCerr("poset_add: poset " + to_string(id) + ", relation(\"" + value1 + "\", \"" +
					value2 + "\") cannot be deleted");
			return false;
		}

		G[v1].erase(v2);
		G_reversed[v2].erase(v1);

        // Sprawdza czy usuniecie relacji nie spowoduje zaburzenia warunkow posetu.
		if (!existsPath(G, v1, v2)) {
            
            // Stworzenie relacji wczesniej wynikajacych z relacji usuwanej.
			for (UL val : G[v2]) {
				if (val != v2) {
					G[v1].insert(val);
					G_reversed[val].insert(v1);
				}
			}
			for (UL val : G_reversed[v1]) {
				if (val != v1) {
					G[val].insert(v2);
					G_reversed[v2].insert(val);
				}

			}

			printToCerr("poset_add: poset " + to_string(id) + ", relation(\"" + value1 + "\", \"" +
					value2 + "\") deleted");
			return true;
		}

		G[v1].insert(v2);
		G_reversed[v2].insert(v1);

		printToCerr("poset_add: poset " + to_string(id) + ", relation(\"" + value1 + "\", \"" +
				value2 + "\") cannot be deleted");
		return false;
	}


	// Sprawdza czy value1 jest przed value2 w posecie.
	bool poset_test(UL id, char const *value1, char const *value2){
		int broken = checkTwoValues(&value1, &value2);
		printToCerr("poset_test(" + to_string(id) + ", \"" + value1 + "\", \"" + value2 + "\")");

		if(!posets().count(id)){
			printToCerr("poset_test: poset " + to_string(id) + " does not exist");
			return false;
		}

		if(broken == 3){
			printToCerr("poset_test: invalid value1 (NULL)");
			printToCerr("poset_test: invalid value2 (NULL)");
			return false;
		}
		else if(broken == 2){
			printToCerr("poset_test: invalid value2 (NULL)");
			return false;
		}
		else if(broken == 1){
			printToCerr("poset_test: invalid value1 (NULL)");
			return false;
		}

		UL valueId1 = getStringId(value1),
				valueId2 = getStringId(value2);


		graph &G = posets()[id].first;

	    if (!G.count(valueId1) || !G.count(valueId2)){
	    	printToCerr("poset_test: poset " + to_string(id) + ", element \"" + value1 + "\" or \"" +
	    			value2 + "\" does not exist");
	    	return false;
	    }

	    if(existsPath(G, valueId1, valueId2)){
			printToCerr("poset_test: poset " + to_string(id) + ", relation(\"" + value1 + "\", \"" +
					value2 + "\") exists");
			return true;
	    }

		printToCerr("poset_test: poset " + to_string(id) + ", relation(\"" + value1 + "\", \"" +
				value2 + "\") does not exist");
		
        return false;
	}

	// Usuwa poset o podanym numerze.
	void poset_clear(UL id){
		printToCerr("poset_clear(" + to_string(id) + ")");
		if(!posets().count(id)){
			printToCerr("poset_clear: poset " + to_string(id) + " does not exist");
			return;
		}
		graph &G = posets()[id].first;
		for (pair <UL, set<UL>> pairs: G)
			convert_erase(rev_converter()[pairs.first], pairs.first);

		posets()[id].first.clear();
		posets()[id].second.clear();

		printToCerr("poset_clear: poset " + to_string(id) + " cleared");
	}
}
