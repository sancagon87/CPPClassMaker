#ifndef DEPENDENCIAS_H
#define DEPENDENCIAS_H

#include <string>
#include <vector>

using namespace std;

class Dependencias
{
private:
	vector<string> depStd;
	vector<string> depCustom;
public:
	Dependencias();
	vector<string>* getDepStd();
	vector<string>* getDepCustom();
	void addDepStd(string);
	void addDepStdM(string);
	void addDepCustom(string);
	void addDepCustomM(string);
	~Dependencias();
};

#endif //DEPENDENCIAS_H