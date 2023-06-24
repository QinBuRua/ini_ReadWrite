#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
using namespace std;

bool is_number(string str){
	stringstream sstr(str);
	double num;
	char ch;
	if(!(sstr>>num)){
		return false;
	}
	if(sstr>>ch){
		return false;
	}
	return true;
}
string trim(string str){
	if(str.empty()){
		return str;
	}
	str.erase(0,str.find_first_not_of(" /r/n"));
	str.erase(str.find_last_not_of(" /r/n")+1);
	return str;
}

class _iniValue{
private:
	string realValue;
public:
	_iniValue(){
		realValue="";
	}
	_iniValue(bool tmp){
		if(tmp==true){
			realValue="true";
		}else{
			realValue="false";
		}
	}
	_iniValue(int tmp){
		stringstream tmpSS;
		tmpSS<<tmp;
		realValue=tmpSS.str();
	}
	_iniValue(double tmp){
		stringstream tmpSS;
		tmpSS<<tmp;
		realValue=tmpSS.str();
	}
	_iniValue(const string & tmp){
		realValue=tmp;
	}
	
	void operator= (bool tmp){
		if(tmp==true){
			realValue="true";
		}else{
			realValue="false";
		}
	}
	void operator= (int tmp){
		stringstream tmpSS;
		tmpSS<<tmp;
		realValue=tmpSS.str();
	}
	void operator= (double tmp){
		stringstream tmpSS;
		tmpSS<<tmp;
		realValue=tmpSS.str();
	}
	void operator= (const string & tmp){
		realValue=tmp;
	}
	
	void set(const string & tmp){
		realValue=tmp;
	}
	
	operator bool(){
		if(is_number(realValue)){
			if(realValue=="0"){
				return false;
			}else{
				return true;
			}
		}else{
			if(realValue=="true"){
				return true;
			}else{
				return false;
			}
		}
	}
	operator int(){
		return atoi(realValue.c_str());
	}
	operator double(){
		return atof(realValue.c_str());
	}
	operator string(){
		return realValue;
	}
};

typedef map<string,_iniValue> _iniSection;

class _iniFile{
private:
	string fileName;
	map<string,_iniSection> iniData;
public:
	_iniFile(){
	}
	string get_fileName(){
		return fileName;
	}
	void clear(){
		fileName="";
		iniData.clear();
	}
	void set(const string &tmpSection,const string &tmpKey,int tmpValue){
		iniData[tmpSection][tmpKey]=tmpValue;
	}
	void set(const string &tmpSection,const string &tmpKey,double tmpValue){
		iniData[tmpSection][tmpKey]=tmpValue;
	}
	void set(const string &tmpSection,const string &tmpKey,const string &tmpValue){
		iniData[tmpSection][tmpKey]=tmpValue;
	}
	void set(const string &tmpSection,const string &tmpKey,bool tmpValue){
		iniData[tmpSection][tmpKey]=tmpValue;
	}
	bool load(const string & tmpFile){
		clear();
		fileName=tmpFile;
		ifstream tmpFin(tmpFile);
		if(tmpFin.fail()){
			tmpFin.close();
			return false;
		}
		string tmpLine,tmpSection,tmpKey,tmpValue;
		while(getline(tmpFin,tmpLine)){
			tmpLine=trim(tmpLine);
			if(tmpLine[0]=='#'||tmpLine[0]==';'||tmpLine==""){
				continue;
			}
			if(tmpLine[0]=='['){
				int rSqlBrkts=tmpLine.find_first_of(']');
				tmpSection=tmpLine.substr(1,rSqlBrkts-1);
				tmpSection=trim(tmpSection);
				iniData[tmpSection]=_iniSection();
			}else{
				int tmpEqalLctn=tmpLine.find_first_of('=');
				tmpKey=trim(tmpLine.substr(0,tmpEqalLctn-1));
				tmpValue=trim(tmpLine.substr(tmpEqalLctn+1));
				if(tmpValue[0]=='"'){
					tmpValue=tmpValue.substr(1,tmpValue.find_first_of('"',1)-1);
				}else{
					if(!is_number(tmpValue)){
						if(tmpValue=="true"||tmpValue=="on"){
							tmpValue="true";
						}else if(tmpValue=="false"||tmpValue=="off"){
							tmpValue="false";
						}else{
							tmpValue="NULL";
						}
					}
				}
				set(tmpSection,tmpKey,tmpValue);
			}
		}
		return true;
	}
	_iniFile(const string &tmpFile){
		load(tmpFile);
	}
	bool has(const string &tmpSection,const string &tmpKey){
		map<string,_iniSection>::const_iterator it = iniData.find(tmpSection);
		if(it!=iniData.end()){
			return (it->second.find(tmpKey) != it->second.end());
		}
		return false;
	}
	bool has(const string &tmpSection){
		return (iniData.find(tmpSection) != iniData.end());
	}
	_iniValue get(const string &tmpSection,const string &tmpKey){
		if(!has(tmpSection,tmpKey)){
			return _iniValue();
		}
		return iniData[tmpSection][tmpKey];
	}
	_iniSection & operator[] (const string & tmp){
		return iniData[tmp];
	}
	void remove(const string &tmpSection,const string &tmpKey){
		map<string,_iniSection>::iterator it=iniData.find(tmpSection);
		if(it != iniData.end()){
			it->second.erase(tmpKey);
		}
	}
	void remove(const string &tmpSection){
		iniData.erase(tmpSection);
	}
	string string_(){
		stringstream sstr;
		for(auto itIniData=iniData.begin(); itIniData!=iniData.end(); itIniData++){
			sstr<<'['<<itIniData->first<<']'<<endl;
			for(auto itIniSection=itIniData->second.begin(); itIniSection!=itIniData->second.end(); itIniSection++){
				if(!is_number(itIniSection->second)){
					if(string(itIniSection->second)!="true"&&string(itIniSection->second)!="false"){
						sstr<<itIniSection->first<<" = \""<<string(itIniSection->second)<<endl<<'\"';
					}
				}
				sstr<<itIniSection->first<<" = "<<string(itIniSection->second)<<endl;
			}
			sstr<<endl;
		}
		return sstr.str();
	}
	void show(){
		cout<<string_();
	}
	bool save(const string &tmpFileName){
		ofstream fout(tmpFileName);
		if(fout.fail()){
			return false;
		}
		fout<<string_();
		return true;
	}
	void assign(const _iniFile &tmp){
		clear();
		for(auto tmpIt=tmp.iniData.begin(); tmpIt!=tmp.iniData.end(); tmpIt++){
			for(auto tmpIter=tmpIt->second.begin(); tmpIter!=tmpIt->second.end(); tmpIter++){
				iniData[tmpIt->first][tmpIter->first]=tmpIter->second;
			}
		}
	}
	void operator= (const _iniFile &tmp){
		assign(tmp);
	}
};
