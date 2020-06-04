#include <algorithm>
#include <iostream>
#include "header.h"
#include <windows.h>
#define COUNT 10

using namespace std;

Node::Node(int data) {  //ustawia nowego node na kolor czerwony i jego dzieci/rodzicow na nulla
	this->data = data;
	color = RED;
	left = right = parent = nullptr;
}

RBTree::RBTree() {
	root = nullptr;
}

int RBTree::getColor(Node*& node) {  //zwraca kolor, null to jakby czarny
	if (node == nullptr)
		return BLACK;

	return node->color;
}

void RBTree::setColor(Node*& node, int color) {
	if (node == nullptr)
		return;

	node->color = color;
}

Node* RBTree::insertBST(Node*& root, Node*& ptr) {
	if (root == nullptr)
		return ptr;

	if (ptr->data < root->data) {  //wstawianie standardowe bst
		root->left = insertBST(root->left, ptr);
		root->left->parent = root;
	}
	else if (ptr->data > root->data) {
		root->right = insertBST(root->right, ptr);
		root->right->parent = root;
	}

	return root;
}

void RBTree::insertValue(int n) {   //tworzymy nowy i wywolujemy funkcje balansujaca
	Node* node = new Node(n);
	root = insertBST(root, node);
	fixInsertRBTree(node);
}

void RBTree::rotateLeft(Node*& ptr) { //rotacja w lewo
	Node* right_child = ptr->right;
	ptr->right = right_child->left;

	if (ptr->right != nullptr)
		ptr->right->parent = ptr;

	right_child->parent = ptr->parent;

	if (ptr->parent == nullptr)
		root = right_child;
	else if (ptr == ptr->parent->left)
		ptr->parent->left = right_child;
	else
		ptr->parent->right = right_child;

	right_child->left = ptr;
	ptr->parent = right_child;
}

void RBTree::rotateRight(Node*& ptr) { //rotacja w prawo
	Node* left_child = ptr->left;
	ptr->left = left_child->right;

	if (ptr->left != nullptr)
		ptr->left->parent = ptr;

	left_child->parent = ptr->parent;

	if (ptr->parent == nullptr)
		root = left_child;
	else if (ptr == ptr->parent->left)
		ptr->parent->left = left_child;
	else
		ptr->parent->right = left_child;

	left_child->right = ptr;
	ptr->parent = left_child;
}

void RBTree::fixInsertRBTree(Node*& ptr) {
	Node* parent = nullptr;
	Node* grandparent = nullptr;
	while (ptr != root && getColor(ptr) == RED && getColor(ptr->parent) == RED) { //niezachowane wlasciwosci
		parent = ptr->parent;
		grandparent = parent->parent;
		if (parent == grandparent->left) {  //rodzic jest lewym dzieckiem 
			Node* uncle = grandparent->right;
			if (getColor(uncle) == RED) {  //jesli wujek jest czerwony 
				setColor(uncle, BLACK); //ustawiamy kolor rodzica i wujka na czarny a dziadka na czerwony
				setColor(parent, BLACK);
				setColor(grandparent, RED); 
				ptr = grandparent; //wskaznik na dziadka
			}
			else {
				if (ptr == parent->right) { //triangle case czyli jestesmy prawym dzieckiem rodzica ktory jest lewym dzieckiemXD 
					rotateLeft(parent); //rotujemy rodzica w lewo(zamiana rodzica z dzieckiem root)
					ptr = parent; //wskaznik na rodzica
					parent = ptr->parent;
				}
				rotateRight(grandparent); //line case czyli jestesmy lewym dzieckiem rodzica ktory jest lewym dzieckiem rotujemy dziadka w prawo
				swap(parent->color, grandparent->color); //zamieniamy kolory rodzica i dziadka
				ptr = parent; //wskaznik na rodzica
			}
		}
		else { //rodzic jest prawym dzieckiem
			Node* uncle = grandparent->left;
			if (getColor(uncle) == RED) { //jesli wujek jest czerwony zmieniamy kolory wujka dziadka rodzica
				setColor(uncle, BLACK);
				setColor(parent, BLACK);
				setColor(grandparent, RED);
				ptr = grandparent; //wskaznik na dziadka
			}
			else {
				if (ptr == parent->left) { //triangle case
					rotateRight(parent); //rotujemy rodzica w prawo
					ptr = parent; //wskaznik na rodzica
					parent = ptr->parent;
				}
				rotateLeft(grandparent); //rotujemy dziadka w lewo
				swap(parent->color, grandparent->color); //zamieniamy kolory rodzica i dziadka
				ptr = parent; //wskaznik na rodzica
			}
		}
	}
	setColor(root, BLACK); //ustawiamy kolor roota na czarny
}

void RBTree::fixDeleteRBTree(Node*& node) {
	if (node == nullptr)
		return;

	if (node == root) {
		root = nullptr;
		return;
	}

	if (getColor(node) == RED || getColor(node->left) == RED || getColor(node->right) == RED) { //jesli node lub jego dzieci sa czerwone
		Node* child = node->left != nullptr ? node->left : node->right; //jesli istnieje lewe dziecko to child to lewe a jesli nie to prawe

		if (node == node->parent->left) { //jesli node to lewe dziecko 
			node->parent->left = child; //przepinamy bachora
			if (child != nullptr) 
				child->parent = node->parent;
			setColor(child, BLACK); //jesli nie null to swap swap i usuwanko 
			delete (node);
		}
		else { //JESLI NIE
			node->parent->right = child;//przepinamy na prawe
			if (child != nullptr)
				child->parent = node->parent;
			setColor(child, BLACK);
			delete (node); //czarn*ch i usuwanko
		}
	}
	else {
		Node* sibling = nullptr; 
		Node* parent = nullptr;
		Node* ptr = node;
		setColor(ptr, DOUBLE_BLACK); //BIG black boi
		while (ptr != root && getColor(ptr) == DOUBLE_BLACK) { //jesli big black boi i nie jest rootem
			parent = ptr->parent;
			if (ptr == parent->left) { 
				sibling = parent->right; //rodzenstwo to po prostu drugie dziecko xd
				if (getColor(sibling) == RED) { //jesli czerwone robimy na czarne a rodzica na czerwony, rotujemy rodzica w lewo
					setColor(sibling, BLACK);
					setColor(parent, RED);
					rotateLeft(parent);
				}
				else {
					if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) { //jesli lewe i prawe dziecko rodzenstwa jest czzarne
						setColor(sibling, RED); //zmikeniamy kolor rodzenstwa na czerwony
						if (getColor(parent) == RED) //jesli rodzic czerwony zmieniamy na czarny
							setColor(parent, BLACK);
						else
							setColor(parent, DOUBLE_BLACK); //jesli nie to robimy big blac boi znow
						ptr = parent; //wskaznik na rodzica
					}
					else {
						if (getColor(sibling->right) == BLACK) { //jesli prawe dziecko rodzenstwa czarne lewe tez robimy czarne a rodzenstwo czerwone
							setColor(sibling->left, BLACK); 
							setColor(sibling, RED);
							rotateRight(sibling); //rotujemy rodzenstwo w prawoo
							sibling = parent->right; //rodzestwem staje sie prawe dziecko rodzica
						}
						setColor(sibling, parent->color); //ustawiamy kolor rodzenstwa na kolor rodzica
						setColor(parent, BLACK); //a rodzica na czarny
						setColor(sibling->right, BLACK); //a prawego dziecka rodzenstwa na czarny tez xd
						rotateLeft(parent); //rotujemy w lewo rodzica
						break;
					}
				}
			}
			else {
				sibling = parent->left; //jesli rodzenstwo jest lewym dzieckiem a my prawym
				if (getColor(sibling) == RED) { //jego kolor jest czerwony
					setColor(sibling, BLACK); //zmieniamy na czarny a rodzica na czerwony i rotujemy rodzica w prawo
					setColor(parent, RED);
					rotateRight(parent);
				}
				else { 
					if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) { //jesli dzieci rodzenstwa sa czarne zmieniamy kolor rodzenstwa na czerwony
						setColor(sibling, RED);
						if (getColor(parent) == RED) //jesli rodzic jest czerwony zmieniamy na czarny
							setColor(parent, BLACK);
						else
							setColor(parent, DOUBLE_BLACK); //jesli nie to zmieniamy na big black bull kozak podwojny cxarny
						ptr = parent; //wskaznik n rodzica
					}
					else {
						if (getColor(sibling->left) == BLACK) { //jesli lewe dziecko rodzenstwa is black to prawe tez robimy czarne a rodzenstwo czerwone
							setColor(sibling->right, BLACK);
							setColor(sibling, RED);
							rotateLeft(sibling); //rotujemy w lewo rodzestwo i ustawiamy na nie lewe dziecko rodzica tzn no wiadomo
							sibling = parent->left; 
						}
						setColor(sibling, parent->color); //ustawiamy kolor rodzenstwa na kolor rodzica kolor rodzica na czarny a lewego dziecka rodzenstwa na czarny tez i rotujemy w prawo rodzica
						setColor(parent, BLACK);
						setColor(sibling->left, BLACK);
						rotateRight(parent);
						break;
					}
				}
			}
		}
		if (node == node->parent->left) //jesli jest lewym dzieckiem to usuwamy po prostu a jesli prawym to tak samo i nullujemy wskazniki na bachory
			node->parent->left = nullptr;
		else
			node->parent->right = nullptr;
		delete(node);
		setColor(root, BLACK); //kolor roota ma byc cxarny wiec no
	}
}

Node* RBTree::deleteBST(Node*& root, int data) { //usuwanko bst
	if (root == nullptr)
		return root;

	if (data < root->data)
		return deleteBST(root->left, data);

	if (data > root->data)
		return deleteBST(root->right, data);

	if (root->left == nullptr || root->right == nullptr)
		return root;

	Node* temp = minValueNode(root->right);
	root->data = temp->data;
	return deleteBST(root->right, temp->data);
}

void RBTree::deleteValue(int data) { //usuwanko wartosci
	Node* node = deleteBST(root, data);
	fixDeleteRBTree(node);
}

void RBTree::inorderBST(Node*& ptr) {
	if (ptr == nullptr)
		return;

	inorderBST(ptr->left);
	cout << ptr->data << " " << ptr->color << endl;
	inorderBST(ptr->right);
}

void RBTree::inorder() {
	inorderBST(root);
}

void RBTree::preorderBST(Node*& ptr) {
	if (ptr == nullptr)
		return;

	cout << ptr->data << " " << ptr->color << endl;
	preorderBST(ptr->left);
	preorderBST(ptr->right);
}

void RBTree::preorder() {
	preorderBST(root);
	cout << "-------" << endl;
}

void RBTree::Print2DUtil(Node * a, int space)
{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		if (a == NULL)
			return;
		space += COUNT;
		Print2DUtil(a->right, space);
		cout << endl;
		for (int i = COUNT; i < space; i++)
			cout << " ";
		if (a->color == RED)
		{
			SetConsoleTextAttribute(h, 12);
			cout << a->data << "\n";
		}
		else if (a->color == BLACK)
		{
			SetConsoleTextAttribute(h, 8);
			cout << a->data << "\n";
		}
		Print2DUtil(a->left, space);
}


Node* RBTree::minValueNode(Node*& node) {

	Node* ptr = node;

	while (ptr->left != nullptr)
		ptr = ptr->left;

	return ptr;
}

Node* RBTree::maxValueNode(Node*& node) {
	Node* ptr = node;

	while (ptr->right != nullptr)
		ptr = ptr->right;

	return ptr;
}

int RBTree::getBlackHeight(Node* node) { //ilosc czarnych node do nulla(droga)
	int blackheight = 0;
	while (node != nullptr) {
		if (getColor(node) == BLACK)
			blackheight++;
		node = node->left;
	}
	return blackheight;
}

