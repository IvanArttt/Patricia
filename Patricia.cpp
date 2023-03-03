#include <vector>
#include <algorithm>
#include <iostream>
#include <bitset>





template<typename T>
class Patricia {
	class TreeNode {
	public:
		TreeNode* _left;
		TreeNode* _right;
		TreeNode* _parent;
		uint8_t _value;
		int _d;
		TreeNode(T value, int d = 0, TreeNode* left = nullptr, TreeNode* right = nullptr, TreeNode* parent = nullptr)
		{
			this->_value = value;
			this->_d = d;
			this->_left = left;
			this->_right = right;
			this->_parent = parent;
		}
	};

	TreeNode* _root;
	int _Size = 0;


	int Cmp(uint8_t v, int d) {
		if (d == -1) return 0;
		if (v & (1 << d))
			return 1;
		else
			return 0;
	}

	/*bool NoEqualTreeNode(TreeNode* a, TreeNode* b) {
		if (a->_value > b->_value)
			return true;
		if (a->_value < b->_value)
			return false;
	}*/

	void Destructor(TreeNode* a) {
		if (a) {
			if (a->_d < (a->_left)->_d) {
				Destructor(a->_left);
			}
			if (a->_d < (a->_right)->_d) {
				Destructor(a->_right);
			}
			delete a;
		}
	}

	void LKP(TreeNode* root, std::vector<T>* A) {
		if (root) {
			LKP(root->_left, A);
			(*A).push_back(root->_value);
			LKP(root->_right, A);
		}

	}

	void PKL(TreeNode* root, std::vector<T>* A) {
		if (root) {
			PKL(root->_right, A);
			(*A).push_back(root->_value);
			PKL(root->_left, A);
		}

	}


	void Print(TreeNode* root, int h) {
		if (root) {
			if (root->_d < (root->_right)->_d) {
				Print(root->_right, h + 1);
			}

			for (int i = 0; i < h; i++) {
				std::cout << "\t";
			}
			if ((root->_value == 0b00000000))
				printf("%c", '0');
			else
				printf("%c", root->_value);
			std::cout << std::endl;
			if (root->_d < (root->_left)->_d) {
				Print(root->_left, h + 1);
			}
		}
	}


	bool CheckingOnRepeats(TreeNode* prev, bool* arr) {
		int count = 0;
		for (int i = 0; i <= 7; i++) {
			if (prev->_code[i] == arr[i])
				count++;
		}
		if (count == 8)
			return true;
		return false;
	}


public:
	Patricia() {
		bool arr[] = { 0,0,0,0,0,0,0,0 };
		this->_root = new TreeNode(0b00000000, -1);
		(this->_root)->_right = (this->_root)->_left = this->_root;
	}




	~Patricia() {
		Destructor(_root);
	}

	bool Empty() {
		return (this->_Size == 0);
	}

	bool Search(uint8_t value)
	{
		TreeNode* prev_node = _root;
		TreeNode* node = _root;

		do
		{
			if (Cmp(value, node->_d) == 0)
			{
				prev_node = node;
				node = node->_left;
				continue;
			}
			if (Cmp(value, node->_d) == 1)
			{
				prev_node = node;
				node = node->_right;
				continue;
			}
		} while (prev_node->_d < node->_d);
		return node->_value == value;
	}

	bool Insert(uint8_t value) {
		TreeNode* new_node = new TreeNode(value);
		int digit = 0;
		TreeNode* current = _root;
		TreeNode* prev_node = _root;

		if (Search(value)) return 0;
		do
		{
			if (Cmp(value, current->_d) == 0)
			{
				prev_node = current;
				current = current->_left;
				continue;
			}
			if (Cmp(value, current->_d) == 1)
			{
				prev_node = current;
				current = current->_right;
				continue;
			}
		} while (prev_node->_d < current->_d);

		for (int i = 0; i < 8; i++)
		{
			if ((bool)(current->_value & (1 << i)) != (bool)(value & (1 << i)))
			{
				digit = i;
				break;
			}
		}

		TreeNode* prev_node1 = _root;
		prev_node1->_d = -1;
		current = _root;

		do
		{
			if (Cmp(value, current->_d) == 0)
			{
				prev_node1 = current;
				current = current->_left;
				continue;
			}
			if (Cmp(value, current->_d) == 1)
			{
				prev_node1 = current;
				current = current->_right;
				continue;
			}
		} while (prev_node1->_d < current->_d && digit>current->_d);

		if ((bool)(new_node->_value & (1 << digit)))
		{
			new_node->_d = digit;
			new_node->_left = current;
			new_node->_right = new_node;
		}
		else
		{
			new_node->_d = digit;
			new_node->_right = current;
			new_node->_left = new_node;
		}

		if (_Size == 0) prev_node1->_left = new_node;
		else if (!(bool)(new_node->_value & (1 << prev_node1->_d))) prev_node1->_left = new_node;
		else prev_node1->_right = new_node;
		_Size++;

		return 1;
	}


	int Size() {
		return this->_Size;
	}

	std::vector<T>* RoundLeftRootRight(TreeNode* root) {
		std::vector<T> A;
		std::vector<T>* arr = &A;
		if (root) {
			LKP(root, arr);
		}
		return arr;
	}


	std::vector<T>* RoundRightRootLeft(TreeNode* root) {
		std::vector<T> A;
		std::vector<T>* arr = &A;
		if (root) {
			PKL(root, arr);
		}
		return arr;
	}





	void Output() {
		Print(this->_root, 0);
	}

	void Output(const char* option) {
		std::vector<T> A;
		std::vector<T>* arr = &A;
		LKP(this->_root, arr);
		try {
			sort(A.begin(), A.end());
			if ((strcmp(option, ">")) == 0) {
				for (int i = A.size() - 1; i >= 0; i--)
				{
					std::cout << A[i] << std::endl;
				}
			}
			if ((strcmp(option, "<")) == 0) {
				for (int i = 0; i < A.size(); i++)
				{
					std::cout << A[i] << std::endl;
				}
			}
		}
		catch (...)
		{
			std::cout << "Something wrong" << std::endl;
		}
	}
};



int main() {
	setlocale(0, "ru");
	Patricia<char> A;
	A.Insert('â');
	//A.Output();
	std::cout << std::endl;
	A.Insert('û');
	//A.Output();
	std::cout << std::endl;
	A.Insert('ï');
	//A.Output();
	std::cout << std::endl;
	A.Insert('å');
	//A.Output();
	std::cout << std::endl;
	A.Insert('é');
	//A.Output();
	std::cout << std::endl;
	A.Insert('÷');
	//A.Output();
	std::cout << std::endl;
	A.Insert('à');
	//A.Output();
	std::cout << std::endl;
	A.Insert('þ');
	std::cout << A.Search('.') << std::endl;
	A.Output();




}