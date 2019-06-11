#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;

#define MAXLEVEL 4  //最高为0 1 2 3这四层
#define HEAD_VALUE -1 //设定头部边界值为-1
#define TAIL_VALUE -1 //设定尾部边界值为-1

class SKNode
{
public:
	int key;
	SKNode* forward[MAXLEVEL];//该点的各层的后续指针

	SKNode() :key(0)
	{
		for (int i = 0; i < MAXLEVEL; ++i)
			forward[i] = nullptr;
	}

	/*bool operator=(SKNode* node)
	{
		if (this->key != node->key)
			return false;
		else
		{
			for (int i = 0; i < MAXLEVEL; ++i)
			{
				if (this->forward[i] != node->forward[i])
					return false;
			}
		}
		return true;
	}*/
};

class SkipList
{
public:
	//SKNode* head = new SKNode();//弱智错误！class这时候不能初始化！只能在构造函数初始化！！！！
	//SKNode* tail = new SKNode();
	SKNode* head;
	SKNode* tail;
	int top_layer;//top_layer是最高的那一层，值为 0 1 2 3 中的一个
	 
	void insert(int val);//我们简化，假定不存在相同数值的节点！
	SKNode* search(int val);
	void remove(int val);
	void destroy();
	void print();//分别打印各层的链表

	SkipList():top_layer(0)//构造函数默认top_layer起始为0
	{
		head = new SKNode();
		head->key = HEAD_VALUE;//-1 ..........-1的形式
		tail = new SKNode();
		tail->key = TAIL_VALUE;

		for (int i = 0; i < MAXLEVEL; ++i)
			head->forward[i] = tail;
	}

	~SkipList()
	{
		destroy();
	}
};

int getRandomIndex(int start, int end)
{
	if (start < end)
	{
		srand(time(NULL));
		return start + rand() % (end - start);
	}
	return start;
}

SKNode* SkipList::search(int val)//同一个类的函数，当然可以任意调用head了！
{
	SKNode* pNode = head;

	for (int i = MAXLEVEL-1; i >= 0; --i)
	{
		while ((pNode->forward[i]->key != TAIL_VALUE) && (pNode->forward[i]->key < val))//要注意边界-1！
			pNode = pNode->forward[i];
	}
	pNode = pNode->forward[0];
	if ((pNode->key != TAIL_VALUE) && (pNode->key == val))
	{
		cout << "find " << val << endl;
		return pNode;
	}
	else
	{
		cout << "find failed" << endl;
	}
	return nullptr;
}

void SkipList::insert(int val)
{
	int index = getRandomIndex(0, MAXLEVEL);
	SKNode* forward_driver[MAXLEVEL];//这个驱动很重要！！！！保存新加入节点在每一层位置上，前面的指针前驱！！ 按理应该用index为参数，但是数组只能用常量做参数，所以我们选择MAXLEVEL ！！
	SKNode* pNew = new SKNode();
	pNew->key = val;

	SKNode* pNode = head;//充当指示器使用

	for (int i = MAXLEVEL-1; i >= 0; --i)//这里只能用MAXLEVEL-1做参数了，定义的是4，要0 1 2 3，所以记得要减一！！！
	{
		while ((pNode->forward[i]->key != TAIL_VALUE) && (pNode->forward[i]->key < val))
			pNode = pNode->forward[i];
		forward_driver[i] = pNode;//正是在下！！
	}
	if (pNode->forward[0]->key == val)
	{
		cout << "Failed, can not insert the same value node" << endl;
		return;
	}
	else
	{
		cout << "Succeed, insert the value: " << val << endl;
		for (int i = index; i >= 0; --i)//这里只能index，MAXLEVEL-1不行，只改变新加入节点自己高度的那些指针
		{
			pNew->forward[i] = forward_driver[i]->forward[i];//还有这里！！
			forward_driver[i]->forward[i] = pNew;
		}
		if (index > top_layer)//更新该对象的最高层数  //同一个类对象的东西，应该可以直接修改top_layer的吧？是的
			top_layer = index;
	}
	return;
}

void SkipList::remove(int val)
{
	if (head->forward[0] == tail)
	{
		cout << "Failed, There has no nodes." << endl;
		return;
	}

	//再次调用search部分的程序，找到要删的点和它之前的驱动
	SKNode* pNode = head;
	SKNode* forward_driver[MAXLEVEL];//这个驱动很重要！！！！保存新加入节点在每一层位置上，前面的指针前驱！！ 按理应该用index为参数，但是数组只能用常量做参数，所以我们选择MAXLEVEL ！！
	for (int i = MAXLEVEL - 1; i >= 0; --i)
	{
		while ((pNode->forward[i]->key != TAIL_VALUE) && (pNode->forward[i]->key < val))//要注意边界-1！
			pNode = pNode->forward[i];
		forward_driver[i] = pNode;//正是在下
	}
	SKNode* pNext = pNode->forward[0];//pNext是要删的点
	if ((pNext->key != TAIL_VALUE) && (pNext->key == val))
	{
		cout << "We will remove the node: " << val << endl;
		for (int i = MAXLEVEL - 1; i >= 0; --i)
		{
			if (pNext->forward[i] != nullptr)//要加限定条件！！！只改变要删除点自己高度的那些指针！！
			{
				forward_driver[i]->forward[i] = pNext->forward[i];//还有这里！！
				pNext->forward[i] = nullptr;
			}
		}
		delete pNext;
	}
	else
	{
		cout << "Failed, There is not that node" << endl;
	}
	//
	return;
}

void SkipList::destroy()
{
	SKNode* pNode = head;
	while (head->forward[0] != nullptr)
	{
		head = head->forward[0];
		for (int i = top_layer; i >= 0; --i)
			pNode->forward[i] = nullptr;
		delete pNode;
	}
	for (int i = top_layer; i >= 0; --i)//差个tail，在这删了
		head->forward[i] = nullptr;
	delete head;
}

void SkipList::print()
{
	for (int i = top_layer; i >= 0; --i)
	{
		cout << endl;
		cout << "Print the " << i << " layer nodes." << endl;
		SKNode* pNode = head;
		while (pNode != nullptr)
		{
			cout << pNode->key << " ";
			pNode = pNode->forward[i];
		}
		cout << endl;
	}
	return;
}

int main()
{
	SkipList* sk=new SkipList();//最好用这样的指针方式来声明
	const int n = 7;
	int num[n] = { 30,15,45,37,11,53,17 };
	cout << "test insert.........." << endl;
	for (int i = 0; i < n; ++i)
		sk->insert(num[i]);
	sk->print();
	cout << "test search................" << endl;
	SKNode* pNode = sk->search(17);
	cout << "test remove.................." << endl;
	sk->remove(30);

	sk->print();
	return 0;
}