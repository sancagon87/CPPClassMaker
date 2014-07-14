#include "Dependencias.h"
#include <iostream>

bool existe(string, vector<string>*);

Dependencias::Dependencias()
{

}
vector<string>* Dependencias::getDepStd()
{
	return &depStd;
}
vector<string>* Dependencias::getDepCustom()
{
	return &depCustom;
}
void Dependencias::addDepStd(string d)
{
	if( !existe(d, &depStd) )
		depStd.push_back(d);
}
void Dependencias::addDepStdM(string d)
{
	d = d.substr(1, d.size() - 2);
	int i = d.find('%');

	while( i != string::npos )
	{
		if( !existe(d.substr(0, i), &depStd) )
			depStd.push_back(d.substr(0, i));
		d = d.substr(i+1, d.size() - i);
		i = d.find('%');
	}
	if( d.size() != 0 )
		depStd.push_back(d);
}
void Dependencias::addDepCustom(string d)
{
	if( !existe(d, &depCustom) )
		depCustom.push_back(d);
}
void Dependencias::addDepCustomM(string d)
{
	d = d.substr(1, d.size() - 2);
	int i = d.find('%');
	
	while( i != string::npos )
	{
		if( !existe(d.substr(0, i), &depCustom) )
			depCustom.push_back(d.substr(0, i));
		d = d.substr(i+1, d.size() - i);
		i = d.find('%');
	}
	if( d.size() != 0 )
		depCustom.push_back(d);
}
Dependencias::~Dependencias()
{
	//STUB
}
bool existe(string s, vector<string>* v)
{
	int l = v->size();

	for (int i = 0; i < l; ++i)
	{
		if (v->at(i) == s)
		{
			cout << "true: " << s << endl;
			return true;
		}
	}
	return false;
}