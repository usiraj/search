/*
 * Data.cpp
 *
 *  Created on: Aug 6, 2012
 *      Author: usama
 */
#include "usjAILHead.h"
#include <iostream>
using namespace std;

namespace usjAIL{

// CLASS Data
Data::Data(){
	kMap = new vector<string>();
	dMap = new map<string,double>();
	iMap = new map<string,int>();
	bMap = new map<string,bool>();
	sMap = new map<string,string>();
	locked = false;
}
Data::Data(const Data &cpy){
	kMap = new vector<string>();
	dMap = new map<string,double>();
	iMap = new map<string,int>();
	bMap = new map<string,bool>();
	sMap = new map<string,string>();
	locked = cpy.locked;
	// populating kmap
	for ( vector<string>::iterator iter1 = cpy.kMap->begin(); iter1 != cpy.kMap->end() ; ++iter1 ){
		this->kMap->push_back(*iter1);
	}
	// populating dMap
	for ( map<string,double>::iterator iter2 = cpy.dMap->begin() ; iter2 != cpy.dMap->end(); ++iter2){
		this->dMap->insert(pair<string,double>(iter2->first,iter2->second));
	}
	// populating iMap
	for ( map<string,int>::iterator iter3 = cpy.iMap->begin() ; iter3 != cpy.iMap->end(); ++iter3){
		this->iMap->insert(pair<string,int>(iter3->first,iter3->second));
	}
	// populating bMap
	for ( map<string,bool>::iterator iter4 = cpy.bMap->begin() ; iter4 != cpy.bMap->end(); ++iter4){
		this->bMap->insert(pair<string,bool>(iter4->first,iter4->second));
	}
	// populating sMap
	for ( map<string,string>::iterator iter5 = cpy.sMap->begin() ; iter5 != cpy.sMap->end(); ++iter5){
		this->sMap->insert(pair<string,string>(iter5->first,iter5->second));
	}
}
Data& Data::operator=(const Data &rhs){
	const int sz = rhs.getSize();
	int ival;
	bool bval;
	string key,sval;
	double dval;
	this->locked = false;	// so that we can add and remove
	if ( !(this->bMap->empty())){
		this->bMap->erase(this->bMap->begin(),this->bMap->end());
	}
	if ( !(this->dMap->empty())){
		this->dMap->erase(this->dMap->begin(),this->dMap->end());
	}
	if ( !(this->iMap->empty())){
		this->iMap->erase(this->iMap->begin(),this->iMap->end());
	}
	if ( !(this->sMap->empty())){
		this->sMap->erase(this->sMap->begin(),this->sMap->end());
	}
	if ( !(this->kMap->empty())){
		this->kMap->erase(this->kMap->begin(),this->kMap->end());
	}
	// erased every thing now start adding
	for ( int i = 0; i < sz ; ++i ){
		key = rhs.getKey(i);
		switch ( rhs.getType(key)){
		case Data::VALUE_BOOLEAN:
			rhs.get(key,&bval);
			this->add(key,bval);
			break;
		case Data::VALUE_FLOAT:
			rhs.get(key,&dval);
			this->add(key,dval);
			break;
		case Data::VALUE_INTEGER:
			rhs.get(key,&ival);
			this->add(key,ival);
			break;
		case Data::VALUE_STRING:
			rhs.get(key,&sval);
			this->add(key,sval);
			break;
		default:
			break;
		}
	}
	if ( rhs.islock() ) locked = true;
	else locked = false;
	return *this;
}
Data::~Data(){
	if ( dMap != NULL ) delete dMap;
	if ( iMap != NULL ) delete iMap;
	if ( bMap != NULL ) delete bMap;
	if ( sMap != NULL ) delete sMap;
	if ( kMap != NULL ) delete kMap;
}
// adding element functions
void Data::add(const string &key,const double value){
	if ( locked ) return;
	// check if key already exists if it exists modify value
	if ( this->keyExists(key) ){
		// if the existing key has same type then modify otherwise return
		if ( this->getType(key) == Data::VALUE_FLOAT ){
			this->modify(key,value);
		} else return;
	} else {
	// if key not exists then add key in kmap and add key and value in dmap
		this->dMap->insert(pair<string,double>(key,value));
		this->kMap->push_back(key);
	}
}
void Data::add(const string &key,const int value){
	if ( locked ) return;
	// check if key already exists if it exists modify value
	if ( this->keyExists(key) ){
		// if the existing key has same type then modify otherwise return
		if ( this->getType(key) == Data::VALUE_INTEGER ){
			this->modify(key,value);
		} else return;
	} else {
	// if key not exists then add key in kmap and add key and value in imap
		this->iMap->insert(pair<string,int>(key,value));
		this->kMap->push_back(key);
	}
}
void Data::add(const string &key,const bool value){
	if ( locked ) return;
	// check if key already exists if it exists modify value
	if ( this->keyExists(key) ){
		// if the existing key has same type then modify otherwise return
		if ( this->getType(key) == Data::VALUE_BOOLEAN ){
			this->modify(key,value);
		} else return;
	} else {
	// if key not exists then add key in kmap and add key and value in bmap
		this->bMap->insert(pair<string,bool>(key,value));
		this->kMap->push_back(key);
	}
}
void Data::add(const string &key, const string &value){
	if ( locked ) return;
	// check if key already exists if it exists modify value
	if ( this->keyExists(key) ){
		// if the existing key has same type then modify otherwise return
		if ( this->getType(key) == Data::VALUE_STRING ){
			this->modify(key,value);
		} else return;
	} else {
	// if key not exists then add key in kmap and add key and value in smap
		this->sMap->insert(pair<string,string>(key,value));
		this->kMap->push_back(key);
	}
}
// getting functions
Data::ValueType Data::getType(const string &key) const {
	if ( this->keyExists(key,Data::VALUE_BOOLEAN) ) return Data::VALUE_BOOLEAN;
	if ( this->keyExists(key,Data::VALUE_FLOAT) ) return Data::VALUE_FLOAT;
	if ( this->keyExists(key,Data::VALUE_INTEGER) ) return Data::VALUE_INTEGER;
	if ( this->keyExists(key,Data::VALUE_STRING) ) return Data::VALUE_STRING;
	return Data::UNSUCCESSFUL;
}
string Data::getKey(const int index) const{
	if ( index < 0 || (unsigned int)index >= this->kMap->size() ) return "";
	return this->kMap->at(index);
}
void Data::get(const string &key,double *valuetoget) const{
	if ( valuetoget == NULL) return;
	if ( this->keyExists(key,Data::VALUE_FLOAT) ){
		*valuetoget = this->dMap->at(key);
	}
}
void Data::get(const string &key,int *valuetoget) const{
	if ( valuetoget == NULL) return;
	if ( this->keyExists(key,Data::VALUE_INTEGER) ){
		*valuetoget = this->iMap->at(key);
	}
}
void Data::get(const string &key,bool *valuetoget) const {
	if ( valuetoget == NULL) return;
	if ( this->keyExists(key,Data::VALUE_BOOLEAN) ){
		*valuetoget = this->bMap->at(key);
	}
}
void Data::get(const string &key,string *valuetoget) const {
	if ( valuetoget == NULL) return;
	if ( this->keyExists(key,Data::VALUE_STRING) ){
		*valuetoget = this->sMap->at(key);
	}
}
// modifying functions
void Data::modify(const string &key, const bool newvalue){
	for ( map<string,bool>::iterator it= this->bMap->begin(); it != this->bMap->end(); ++it){
		if ( Data::compareSTLStrings(it->first,key) ){
			// found in store
			it->second = newvalue;	// value changed
			break;
		}
	}
}
void Data::modify(const string &key,const int newvalue){
	for ( map<string,int>::iterator it= this->iMap->begin(); it != this->iMap->end(); ++it){
		if ( Data::compareSTLStrings(it->first,key) ){
			// found in store
			it->second = newvalue;	// value changed
			break;
		}
	}
}
void Data::modify(const string &key, const double newvalue){
	for ( map<string,double>::iterator it= this->dMap->begin(); it != this->dMap->end(); ++it){
		if ( Data::compareSTLStrings(it->first,key) ){
			// found in store
			it->second = newvalue;	// value changed
			break;
		}
	}
}
void Data::modify(const string &key, const string &newvalue){
	for ( map<string,string>::iterator it= this->sMap->begin(); it != this->sMap->end(); ++it){
		if ( Data::compareSTLStrings(it->first,key) ){
			// found in store
			it->second = newvalue;	// value changed
			break;
		}
	}
}
// removing functions
void Data::clear(ValueType oftype){
	std::string tempkey;
	if (this->locked) return;
	for (int i = 0; i < this->getSize();){
		tempkey = this->getKey(i);
		if ( this->getType(tempkey) == oftype ){
			this->remove(tempkey);
		} else {
			++i;
		}
	}
}
void Data::remove(const string &key){
	if ( locked ) return;
	if ( this->keyExists(key) ){
		switch ( this->getType(key)){
		case Data::VALUE_BOOLEAN:
			this->bMap->erase(key);
			break;
		case Data::VALUE_FLOAT:
			this->dMap->erase(key);
			break;
		case Data::VALUE_INTEGER:
			this->iMap->erase(key);
			break;
		case Data::VALUE_STRING:
			this->sMap->erase(key);
			break;
		default:
			break;
		}
		vector<string>::iterator itk = this->kMap->begin();
		for ( ; itk != this->kMap->end(); ++itk){
			if ( Data::compareSTLStrings(key,*itk)) break;
		}
		this->kMap->erase(itk);
	}
}
// searching functions
bool Data::keyExists(const string &key) const{
	for ( vector<string>::iterator it = this->kMap->begin(); it != this->kMap->end(); ++it){
		if ( Data::compareSTLStrings(key,*it)){
			return true;	// yes key exists
		}
	}
	return false;
}
bool Data::keyExists(const string &key, ValueType oftype) const {
	switch ( oftype ){
	case Data::VALUE_BOOLEAN:
		for ( map<string,bool>::iterator it = this->bMap->begin(); it != this->bMap->end(); ++ it){
			if ( Data::compareSTLStrings(key,it->first) ) return true;
		}
		break;
	case Data::VALUE_INTEGER:
		for ( map<string,int>::iterator it = this->iMap->begin(); it != this->iMap->end(); ++ it){
			if ( Data::compareSTLStrings(key,it->first) ) return true;
		}
		break;
	case Data::VALUE_FLOAT:
		for ( map<string,double>::iterator it = this->dMap->begin(); it != this->dMap->end(); ++ it){
			if ( Data::compareSTLStrings(key,it->first) ) return true;
		}
		break;
	case Data::VALUE_STRING:
		for ( map<string,string>::iterator it = this->sMap->begin(); it != this->sMap->end(); ++ it){
			if ( Data::compareSTLStrings(key,it->first) ) return true;
		}
		break;
	default:
		break;
	}
	return false;
}
bool Data::valueEqual(const string &key, const double cmpValue) const {
	for ( map<string,double>::iterator it = this->dMap->begin(); it != this->dMap->end(); ++it){
		if ( Data::compareSTLStrings(key,it->first) ){
			// key is same
			if ( cmpValue == it->second ){
				// value is also same
				return true;
			} else {
				// since key can not be duplicate
				break;
			}
		}
	}
	return false;
}
bool Data::valueEqual(const string &key, const int cmpValue) const {
	for ( map<string,int>::iterator it = this->iMap->begin(); it != this->iMap->end(); ++it){
			if ( Data::compareSTLStrings(key,it->first) ){
				// key is same
				if ( cmpValue == it->second ){
					// value is also same
					return true;
				} else {
					// since key can not be duplicate
					break;
				}
			}
		}
		return false;
}
bool Data::valueEqual(const string &key, const bool cmpValue) const {
	for ( map<string,bool>::iterator it = this->bMap->begin(); it != this->bMap->end(); ++it){
			if ( Data::compareSTLStrings(key,it->first) ){
				// key is same
				if ( cmpValue == it->second ){
					// value is also same
					return true;
				} else {
					// since key can not be duplicate
					break;
				}
			}
		}
		return false;
}
bool Data::valueEqual(const string &key, const string &cmpValue) const {
	for ( map<string,string>::iterator it = this->sMap->begin(); it != this->sMap->end(); ++it){
			if ( Data::compareSTLStrings(key,it->first) ){
				// key is same
				if ( cmpValue == it->second ){
					// value is also same
					return true;
				} else {
					// since key can not be duplicate
					break;
				}
			}
		}
		return false;
}
void Data::lock() const {
	locked = true;
}
void Data::unlock(){
	locked = false;
}
bool Data::islock() const { return locked; }
bool Data::sameType(const Data &cmpData ) const {
	const int sz = this->getSize();
	string key;
	if ( sz != cmpData.getSize() ) return false;
	if ( (signed int)this->dMap->size() != cmpData.getSize(Data::VALUE_FLOAT) ) return false;
	if ( (signed int)this->iMap->size() != cmpData.getSize(Data::VALUE_INTEGER) ) return false;
	if ( (signed int)this->bMap->size() != cmpData.getSize(Data::VALUE_BOOLEAN) ) return false;
	if ( (signed int)this->sMap->size() != cmpData.getSize(Data::VALUE_STRING) ) return false;
	// checking key names
	for ( int i = 0; i < sz ; ++i){
		key = this->getKey(i);
		if ( this->getType(key) != cmpData.getType(key) ) return false; // key type different
	}
	// no problem found so far
	return true;
}
bool Data::operator==(const Data &cmpData) const{
	if ( !(this->sameType(cmpData)) ) return false;
	const int sz = this->getSize();
	string key;
	bool bval1,bval2;
	double dval1,dval2;
	int ival1,ival2;
	string sval1,sval2;
	for ( int i = 0; i < sz ; ++i ){
		key = this->kMap->at(i);
		switch ( this->getType(key) ){
		case Data::VALUE_BOOLEAN:
			bval1 = this->bMap->at(key);	cmpData.get(key,&bval2);
			if ( bval1 != bval2 ) return false;
			break;
		case Data::VALUE_FLOAT:
			dval1 = this->dMap->at(key);	cmpData.get(key,&dval2);
			if ( dval1 != dval2 ) return false;
			break;
		case Data::VALUE_INTEGER:
			ival1 = this->iMap->at(key);	cmpData.get(key,&ival2);
			if ( ival1 != ival2) return false;
			break;
		case Data::VALUE_STRING:
			sval1 = this->sMap->at(key);	cmpData.get(key,&sval2);
			if ( !(Data::compareSTLStrings(sval1,sval2))) return false;
			break;
		default:
			break;
		}
	}
	// no problem so far so they must be equal
	return true;
}
bool Data::operator!=(const Data &cmpData) const{
	return !((*this) == cmpData);
}
// size functions
int Data::getSize() const{
	return this->kMap->size();
}
int Data::getSize(Data::ValueType oftype) const{
	switch ( oftype ){
	case Data::VALUE_BOOLEAN :
		return this->bMap->size();
		break;
	case Data::VALUE_FLOAT :
		return this->dMap->size();
		break;
	case Data::VALUE_INTEGER:
		return this->iMap->size();
		break;
	case Data::VALUE_STRING:
		return this->sMap->size();
	default:
		return Data::UNSUCCESSFUL;
	}
}
// static functions
bool Data::compareSTLStrings(const string &str1,const string &str2){
	if ( str1.length() != str2.length()) return false;
	for ( unsigned int i = 0; i < str1.length() ; ++i ){
		if ( str1.at(i) != str2.at(i)) return false;
	}
	return true;
}
// debug print
void Data::print() const{
	string key;
	bool bval;	string sval; double dval; int ival;
	cout << "------------------------------------" << endl;
	cout << "Size of Data Map : " << this->getSize() << endl;
	cout << "Size of Integers : " << this->getSize(State::VALUE_INTEGER) << endl;
	cout << "Size of Booleans : " << this->getSize(State::VALUE_BOOLEAN) << endl;
	cout << "Size of Floating Numbers : " << this->getSize(State::VALUE_FLOAT) << endl;
	cout << "Size of Strings : " << this->getSize(State::VALUE_STRING) << endl;
	for ( int i = 0; i < this->getSize() ; ++i ){
		key = this->getKey(i);
		cout << i << " Key : " << key;
		switch ( this->getType(key) ){
		case State::VALUE_BOOLEAN:
			this->get(key,&bval);
			cout << " - Value : " << bval;
			cout << " - Type : Boolean" << endl;
			break;
		case State::VALUE_FLOAT:
			this->get(key,&dval);
			cout << " - Value : " << dval;
			cout << " - Type : Float" << endl;
			break;
		case State::VALUE_INTEGER:
			this->get(key,&ival);
			cout << " - Value : " << ival;
			cout << " - Type : Integer" << endl;
			break;
		case State::VALUE_STRING:
			this->get(key,&sval);
			cout << " - Value : " << sval;
			cout << " - Type : String" << endl;
			break;
		default:
			break;
		}
	}
	cout << "------------------------------------" << endl;
}
// name space end
}
