#include <algorithm>
#include "header.h"
#include <iostream>
#include <windows.h>
#define COUNT 10

using namespace std;

int main() {
	int data;
	RBTree rbTree1;

	
	/*cin >> data;
	cout << "WSTAWIAMY  " << data << endl;

	while(data!=0)
	{
		rbTree1.insertValue(data);
		rbTree1.Print2DUtil();
		cin >> data;
		cout << "WSTAWIAMY  " << data << endl;
	}*/

	rbTree1.insertValue(3);
	rbTree1.insertValue(18);
	rbTree1.insertValue(7);
	rbTree1.insertValue(9);
	rbTree1.insertValue(1);
	rbTree1.insertValue(12);
	rbTree1.insertValue(5);
	rbTree1.insertValue(4);
	rbTree1.preorder();
	rbTree1.Print2DUtil();
	cout << "USUWAMY COSTAM COS TAM" << endl;
	rbTree1.deleteValue(12);
	rbTree1.Print2DUtil();

	system("PAUSE");
	return 0;
}