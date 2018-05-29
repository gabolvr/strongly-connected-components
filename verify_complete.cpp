#include <iostream>
#include <map>

using namespace std;

int main(){
	int n;
	map<int, int> quant;
	while (cin >> n){
		quant[n]++;
	}
	int i = 0;
	for(map<int, int>::iterator it = quant.begin(); it != quant.end(); it++, i++){
		if (i != it->first)
			cout << "WARNING ";
		cout << it->first << " " << it->second << endl;
	}
	return 0;
}