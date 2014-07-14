#include <string>
#include <iostream>
#include <fstream>

#include "Dependencias.h"

using namespace std;

bool checkExist(string);
void createClass(string name, Dependencias* dep = NULL);
void createCPP(string);
string toUpper(string);
bool isOption(string);
void printHelp();

vector<string> padres;
string opciones[] = {"-i", "-ds", "-dc", "-uns", "-v"};
#define LARGO_OPCIONES 5
#define I 		1  // 2^0, bit 0
#define DS		2  // 2^1, bit 1
#define DC 		4  // 2^2, bit 2
#define UNS 	8  // 2^3, bit 3
#define V 		16 // 2^4, bit 4
unsigned char flags = 0;

int main(int argc, char * argv[])
{
	bool optEnd = false, dcfirst = false, dsfirst = false, error = false;
	string in;
	string in2;
	Dependencias* dep = new Dependencias();
	vector<int> deps;
	if (argc == 1)
	{
		cout << "Nombre de la clase: ";
		getline(cin, in, '\n');
		if( !checkExist(in) )
		{
			cout << "Archivo no creado! Saliendo..." << endl;
			return 0;
		}
		do
		{
			cout << "Interface? (s/n)" << endl;
			cout << "> ";
			getline(cin, in2, '\n');
			if( in2 == "s" )
			{
				flags |= I;
				error = false;
			} else {
				cout << "error! Ingrese s o n." << endl;
				error = true;
			}
		}while(error);

		createClass(in);

	} else {

		if( (string(argv[1]) == "--help")||(string(argv[1]) == "-h") )
		{
			printHelp();
			return 0;
		}
		for( int i = 1; i < argc-1; i++ )
		{
			if( (argv[i][0] == '-') )
			{
				if( !isOption(string(argv[i])) )
				{
					cout << "Invalid option: " << argv[i] << endl;
					cout << "ClassMaker --help for help" << endl;
					return 0;
				}
				if( (string(argv[i]) == "-i")&&( (flags & I) != I ) )
					flags |= I;
				if( (string(argv[i]) == "-uns")&&( (flags & UNS) != UNS ) )
					flags |= UNS;
				if( (string(argv[i]) == "-ds")&&( (flags & DS) != DS ) ) 
					flags |= DS;
				if( (string(argv[i]) == "-dc")&&( (flags & DC) != DC ) )
					flags |= DC;
				if( (string(argv[i]) == "-v")&&( (flags & V) != V ) )
					flags |= V;
				if( (string(argv[i]) == "-ds")&&(!dcfirst) )
				{
					dsfirst = true;
				}
				if( (string(argv[i]) == "-dc")&&(!dsfirst) )
				{
					dcfirst = true;
				}
			} else {
				if( argv[i][0] == '%' )
				{
					padres.push_back(argv[i]);
					dep->addDepCustom( string(argv[i]).substr(1, string(argv[i]).size() - 1 ) );
				}
				if( argv[i][0] == '[' )
					deps.push_back(i);
			}
		}
		if( (argv[argc-1][0] == '-')||(argv[argc-1][0] == '[')||(argv[argc-1][0] == '%') )
		{
			cout << "No file name!" << endl;
			cout << "ClassMaker --help for help" << endl;
			return 0;
		}
		if( ((flags & DC)==DC)||((flags & DS)==DS) )
		{
			cout << "largo deps: " << deps.size() << endl;
			if( (deps.size() > 0)&&(deps.size() < 3) )
			{
				if(dsfirst)
				{
					dep->addDepStdM(argv[deps[0]]);
					if( (flags & DC) == DC )
						dep->addDepCustomM(argv[deps[1]]);
				} else if(dcfirst) {
					dep->addDepCustomM(argv[deps[0]]);
					if( (flags & DS) == DS )
						dep->addDepStdM(argv[deps[1]]);
				}
			} else {
				cout << "Sintaxis error.\nCorrect usage: ClassMaker [options] [[dependencies]] className" << endl;
				return 0;
			}
		}

		if( !checkExist(argv[argc-1]) )
		{
			cout << "Archivo no creado! Saliendo..." << endl;
			return 0;
		}
		createClass(argv[argc-1], dep);

		//cout << "Error! Uso correcto: ClassMaker [options] className" << endl;
	}

	return 0;
}
bool checkExist(string in)
{
	ifstream checkFile( (in+".h").c_str(), ios::binary);
	if( checkFile.is_open() )
	{
		checkFile.close();
		do
		{
			cout << "Ya existe una clase con ese nombre. Reemplazar? (s/n)" << endl;
			cout << "> ";
			getline(cin, in, '\n');
			if( in == "n" )
				return false;

			if( in == "s" )
			{
				return true;
			} else {
				cout << "Error! Ingrese s o n." << endl;
			}

		}while(true);
	}
	checkFile.open( (in+".cpp").c_str(), ios::binary);
	if( checkFile.is_open() )
	{
		checkFile.close();
		do
		{	
			cout << "Ya existe una clase con ese nombre. Reemplazar? (s/n)" << endl;
			cout << "> ";
			getline(cin, in, '\n');
			if( in == "n" )
				return false;

			if( in == "s" )
			{
				return true;
			} else {
				cout << "Error! Ingrese s o n." << endl;
			}
		}while(true);
	}
	return true;
}
void createClass(string name, Dependencias* dep)
{
	ofstream newFile( (name+".h").c_str() );
	vector<string>* depS;
	vector<string>* depC;

	//.H
	newFile << "#ifndef " << toUpper(name) << "_H" << endl;
	newFile << "#define " << toUpper(name) << "_H" << endl;

	if( dep != NULL )
	{
		depS = dep->getDepStd();
		depC = dep->getDepCustom();
		//DEPENDENCIAS
		if( depS->size() )
		{//STD
			newFile << endl;
			newFile << "//includes std" << endl;
			for( int i = 0; i < depS->size(); i++ )
			{
				newFile << "#include <" << depS->at(i) << ">" << endl;
			}
		}
		if( depC->size() )
		{//CUSTOM
			newFile << endl;
			newFile << "//includes custom" << endl;
			for( int i = 0; i < depC->size(); i++ )
			{
				newFile << "#include \"" << depC->at(i) << ".h\"" << endl;
			}
		}
	}
	newFile << endl;
	if( (flags & UNS) == UNS )
		newFile << "using namespace std;" << endl;
	newFile << endl;
	newFile << "class " << name;
	if( padres.size() )
	{

		newFile << " : " << padres[0].substr(1, padres[0].size() - 1);
		for( int i = 1; i < padres.size(); i++ )
		{
			newFile << ", " << padres[i].substr(1, padres[i].size() - 1);
		}
	}
	newFile << "\n{" << endl;
	if( (flags & I) != I )
		newFile << "private:" << endl;
	newFile << "public:" << endl;
	//CONSTRUCTOR
	if( (flags & I) != I  )
		newFile << "\t" << name << "();" << endl;
	if( ((flags & I) == I)||((flags & V) == V) )
	{
		newFile << "\tvirtual ~" << name << "();" << endl;	
	} else {
		newFile << "\t~" << name << "();" << endl;
	}
	newFile << "};" << endl;
	newFile << "#endif //" << toUpper(name) << "_H" << endl;

	newFile.close();

	createCPP(name);
}
void createCPP(string name)
{
	ofstream newFile( (name+".cpp").c_str() );

	newFile << "#include \"" << name << ".h\"" << endl;
	//CONSTRUCTOR
	if( (flags & I) != I )
	{	
		newFile << name << "::" << name << "()" << endl;
		newFile << "{" << endl;
		newFile << "\t//STUB" << endl;
		newFile << "}" << endl;
	}
	//DESTRUCTOR

	newFile << name << "::~" << name << "()" << endl;
	newFile << "{" << endl;
	newFile << "\t//STUB" << endl;
	newFile << "}" << endl;

	newFile.close();
}
bool isOption(string o)
{
	for( int i = 0; i < LARGO_OPCIONES; i++ )
	{
		if( o == opciones[i] )
			return true;
	}

	return false;
}
string toUpper(string s)
{
	int l = s.size(), offset = 'A' - 'a';
	char auxC;
	string auxS;

	for( int i = 0; i < l; i++ )
	{
		auxC = s[i];
		if( (auxC <= 'z') && (auxC >= 'a') )
			auxC = auxC + offset;

		auxS += auxC;
	}
	return auxS;
}
void printHelp()
{
	cout << "--== ClassMaker ==--" << endl;
	cout << "Uso: ClassMaker [options] [[dependencia1$dependencia2]] className" << endl;
	cout << "Crea un archivo className.h, y si no es una interfaz (-i) un archivo className.cpp" << endl;
	cout << "Ejemplo: ClassMaker -ds [string$vector] myClass -> Crea myClass.h y myClass.cpp" << endl;
	cout << "Opciones:" << endl;
	cout << "  -i - Interfaz" << endl;
	cout << "  -ds - dependencias de la libreria estandard" << endl;
	cout << "  -dc - dependencias custom" << endl;
	cout << "  -uns - Using namespace std" << endl;
}