#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;

#define MAXLEVEL 4  //���Ϊ0 1 2 3���Ĳ�
#define HEAD_VALUE -1 //�趨ͷ���߽�ֵΪ-1
#define TAIL_VALUE -1 //�趨β���߽�ֵΪ-1

class SKNode
{
public:
	int key;
	SKNode* forward[MAXLEVEL];//�õ�ĸ���ĺ���ָ��

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
	//SKNode* head = new SKNode();//���Ǵ���class��ʱ���ܳ�ʼ����ֻ���ڹ��캯����ʼ����������
	//SKNode* tail = new SKNode();
	SKNode* head;
	SKNode* tail;
	int top_layer;//top_layer����ߵ���һ�㣬ֵΪ 0 1 2 3 �е�һ��
	 
	void insert(int val);//���Ǽ򻯣��ٶ���������ͬ��ֵ�Ľڵ㣡
	SKNode* search(int val);
	void remove(int val);
	void destroy();
	void print();//�ֱ��ӡ���������

	SkipList():top_layer(0)//���캯��Ĭ��top_layer��ʼΪ0
	{
		head = new SKNode();
		head->key = HEAD_VALUE;//-1 ..........-1����ʽ
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

SKNode* SkipList::search(int val)//ͬһ����ĺ�������Ȼ�����������head�ˣ�
{
	SKNode* pNode = head;

	for (int i = MAXLEVEL-1; i >= 0; --i)
	{
		while ((pNode->forward[i]->key != TAIL_VALUE) && (pNode->forward[i]->key < val))//Ҫע��߽�-1��
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
	SKNode* forward_driver[MAXLEVEL];//�����������Ҫ�������������¼���ڵ���ÿһ��λ���ϣ�ǰ���ָ��ǰ������ ����Ӧ����indexΪ��������������ֻ���ó�������������������ѡ��MAXLEVEL ����
	SKNode* pNew = new SKNode();
	pNew->key = val;

	SKNode* pNode = head;//�䵱ָʾ��ʹ��

	for (int i = MAXLEVEL-1; i >= 0; --i)//����ֻ����MAXLEVEL-1�������ˣ��������4��Ҫ0 1 2 3�����Լǵ�Ҫ��һ������
	{
		while ((pNode->forward[i]->key != TAIL_VALUE) && (pNode->forward[i]->key < val))
			pNode = pNode->forward[i];
		forward_driver[i] = pNode;//�������£���
	}
	if (pNode->forward[0]->key == val)
	{
		cout << "Failed, can not insert the same value node" << endl;
		return;
	}
	else
	{
		cout << "Succeed, insert the value: " << val << endl;
		for (int i = index; i >= 0; --i)//����ֻ��index��MAXLEVEL-1���У�ֻ�ı��¼���ڵ��Լ��߶ȵ���Щָ��
		{
			pNew->forward[i] = forward_driver[i]->forward[i];//���������
			forward_driver[i]->forward[i] = pNew;
		}
		if (index > top_layer)//���¸ö������߲���  //ͬһ�������Ķ�����Ӧ�ÿ���ֱ���޸�top_layer�İɣ��ǵ�
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

	//�ٴε���search���ֵĳ����ҵ�Ҫɾ�ĵ����֮ǰ������
	SKNode* pNode = head;
	SKNode* forward_driver[MAXLEVEL];//�����������Ҫ�������������¼���ڵ���ÿһ��λ���ϣ�ǰ���ָ��ǰ������ ����Ӧ����indexΪ��������������ֻ���ó�������������������ѡ��MAXLEVEL ����
	for (int i = MAXLEVEL - 1; i >= 0; --i)
	{
		while ((pNode->forward[i]->key != TAIL_VALUE) && (pNode->forward[i]->key < val))//Ҫע��߽�-1��
			pNode = pNode->forward[i];
		forward_driver[i] = pNode;//��������
	}
	SKNode* pNext = pNode->forward[0];//pNext��Ҫɾ�ĵ�
	if ((pNext->key != TAIL_VALUE) && (pNext->key == val))
	{
		cout << "We will remove the node: " << val << endl;
		for (int i = MAXLEVEL - 1; i >= 0; --i)
		{
			if (pNext->forward[i] != nullptr)//Ҫ���޶�����������ֻ�ı�Ҫɾ�����Լ��߶ȵ���Щָ�룡��
			{
				forward_driver[i]->forward[i] = pNext->forward[i];//���������
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
	for (int i = top_layer; i >= 0; --i)//���tail������ɾ��
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
	SkipList* sk=new SkipList();//�����������ָ�뷽ʽ������
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