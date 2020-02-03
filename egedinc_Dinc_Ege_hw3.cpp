#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// Ege Toker Dinc 24839 cs204 hw3   The Round Robin Assignment

using namespace std;

struct courseNode{
	string courseName;
	int capacity;
	courseNode *next;

	courseNode ()
	{
		courseName = "";
		capacity = 0;
		next = nullptr;
	}

	courseNode(string name, int cap, courseNode *ptr = nullptr)
	{
		courseName = name;
		capacity = cap;
		next = ptr;
	}
};

struct lectureNode{
	string lectureName;
	lectureNode *next;

	lectureNode()
	{
		lectureName = "";
		next = NULL;
	}

	lectureNode(string name, lectureNode* ptr=NULL)
	{
		lectureName = name;
		next = ptr;
	}
};

struct Student{
	string name;
	int id;
	lectureNode *lectures;

	Student()
	{
		name = "";
		id = 0;
		lectures = NULL;
	}

	Student(string n, int i, lectureNode *ptr = NULL)
	{
		name = n;
		id = i;
		lectures = ptr;
	}

};

struct StackNode
{
	string value;
	StackNode *next;
};

class DynIntStack
{
private:
	StackNode *top;

public:
	DynIntStack(void);
	void push(string);
	void pop(string &);
	bool isEmpty(void);
};

DynIntStack::DynIntStack()
{
	top = nullptr; 
}

void DynIntStack::push(string num)
{
	StackNode *newNode;
	// Allocate a new node & store Num
	newNode = new StackNode;
	newNode->value = num;

	// If there are no nodes in the list
	// make newNode the first node
	if (isEmpty())
	{
		top = newNode;
		newNode->next = nullptr;
	}
	else	// Otherwise, insert NewNode before top
	{
		newNode->next = top;
		top = newNode;
	}
}

void DynIntStack::pop(string &num)
{
	StackNode *temp;

	if (isEmpty())
	{
		cout << "The stack is empty.\n";
	}
	else	// pop value off top of stack
	{
		num = top->value;
		temp = top->next;
		delete top;
		top = temp;
	}
}

bool DynIntStack::isEmpty(void)
{
	bool status;

	if (top == nullptr)
		status = true;
	else
		status = false;

	return status;
}

struct QueueNode
{
	Student value;
	QueueNode *next;
	QueueNode(Student num, QueueNode *ptr = nullptr)
	{
		value = num;
		next = ptr;
	}
};

class DynIntQueue
{
private:
	// These track the front and rear of the queue.
	QueueNode *front;
	QueueNode *rear;	
public:
	DynIntQueue(); // Constructor.	
	// Member functions.
	void enqueue(Student);
	void dequeue(Student &);
	bool isEmpty() const;     
	void clear();
};

DynIntQueue::DynIntQueue()
{
	front = nullptr;
	rear = nullptr;   
}

void DynIntQueue::enqueue(Student num)
{       
	if (isEmpty())   //if the queue is empty
	{	//make it the first element
		front = new QueueNode(num);
		rear = front;
	}
	else  //if the queue is not empty
	{	//add it after rear
		rear->next = new QueueNode(num);
		rear = rear->next;
	} 

}

void DynIntQueue::dequeue(Student &num)
{
	QueueNode *temp;
	if (isEmpty())
	{
		cout << "Attempting to dequeue on empty queue, exiting program...\n";
		exit(1);
	}
	else //if the queue is not empty
	{	//return front's value, advance front and delete old front
		num = front->value;
		temp = front;
		front = front->next;
		delete temp;      
	}
}

bool DynIntQueue::isEmpty() const
{
	if (front == nullptr)
		return true;
	else 
		return false;
}

void DynIntQueue::clear()
{
	Student value;   // Dummy variable for dequeue

	while(!isEmpty())
		dequeue(value); //delete all elements
	#ifdef _DEBUG
		cout << "queue cleared\n";
	#endif
}

DynIntQueue q;

courseNode *head = nullptr;

// identifies the tag on the given line and checks if the matching tag is correct
string GetTag(string s)
{
	int start, end;
	if(s.find("/") != string::npos){
		if(s.find("<", 2) != string::npos){
			start = s.find("<");
			end = s.find(">");
		}
		else{
			start = s.find("/");
			end = s.find(">");
		}
	}
	else{
		start = s.find("<");
		end = s.find(">");
	}
	string tag = s.substr(start+1, (end-start-1)), tag2;
	
	if(s.find("/", end+1 && s.find(">", end+1))){
		if(!s.find(tag, end)){
			cout << "Invalid XML format!.. Exiting." <<  endl;
			exit(0);
		}
	}
	return tag;
}

//removes non-alpha caracters and returns the info
string Strip(string s)
{
	int head, tail;
	head = s.find(">");
	tail = s.find("<", head);
	string item = s.substr(head+1, (tail-head-1));
	return item;
}

//asks for input, does input check and parses the files into a queue and linked list
void TakeInputAndParse()
{
	string studentFileName, lecturesFileName;
	ifstream input1, input2;

	while(true){
		cout << "Please enter the name of the Student XML file: ";
		cin >> studentFileName;
		
		input1.open(studentFileName);
	
		if(input1.fail())
			cout << "Invalid file." << endl;
		else
			break;
	}

	while(true){
		cout << "Please enter the name of the Lectures XML file: ";
		cin >> lecturesFileName;

		input2.open(lecturesFileName);

		if(input2.fail())
			cout << "Invalid file." << endl;
		else
			break;
	}

	DynIntStack studentStack, lectureStack, temp;
	string s, tag;

	while(getline(input1, s)){
		temp.push(s);
	}
	while(!temp.isEmpty()){
		temp.pop(s);
		studentStack.push(s);
	}

	while (getline(input2, s)){
		temp.push(s);
	}
	while(!temp.isEmpty()){
		temp.pop(s);
		lectureStack.push(s);
	}

	while(!studentStack.isEmpty()){
		studentStack.pop(s);
		tag = GetTag(s);
		if(tag == "student"){
			Student stu;
			studentStack.pop(s);
			stu.name = Strip(s);
			studentStack.pop(s);
			stu.id = stoi(Strip(s));
			
			studentStack.pop(s);
			tag = GetTag(s);
			lectureNode *tail = stu.lectures = new lectureNode(Strip(s), nullptr);
			while(true){
				studentStack.pop(s);
				tag = GetTag(s);
				if(tag == "student")
					break;
				tail = tail->next = new lectureNode(Strip(s), nullptr);
			}
			q.enqueue(stu);
		}
	}
	string name;
	int cap;
	while(!lectureStack.isEmpty()){
		lectureStack.pop(s);
		tag = GetTag(s);
		if(tag == "lecture"){
			lectureStack.pop(s);
			tag = GetTag(s);
			name = Strip(s);
			lectureStack.pop(s);
			tag = GetTag(s);
			cap = stoi(Strip(s));
			head = new courseNode(name, cap, head);
			lectureStack.pop(s);
		}
	}
}

// where assignment happens
void DoRoundRobin(){

	Student st;
	string className;
	courseNode *ptr;
	lectureNode *tmp;

	while (!q.isEmpty()){
		q.dequeue(st);
		className = st.lectures->lectureName;
		ptr = head;
		bool found = false;
		while (ptr){
			if (ptr->courseName == className){
				found = true;
				break;
			}
			ptr = ptr->next;
		}
		if(found){
			if(ptr->capacity == 0){
				cout << className << " can not be assigned to " << st.name << "(" << st.id << ")" << endl;
			}
			else{
				ptr->capacity--;
				cout << className << " is assigned to " << st.name << "(" << st.id << ")" << endl;
			}
			if(st.lectures->next != nullptr){
				tmp = st.lectures;
				st.lectures = st.lectures->next;
				delete tmp;
				q.enqueue(st);
			}
			else{
				delete st.lectures;
			}
		}
		else{
			cout << className << " can not be assigned to " << st.name << "(" << st.id << ")" << endl;
			if(st.lectures->next != nullptr){
				tmp = st.lectures;
				st.lectures = st.lectures->next;
				delete tmp;
				q.enqueue(st);
			}
			else{
				delete st.lectures;
			}
		}
	}
} 

int main()
{
	TakeInputAndParse();
	DoRoundRobin();

	return 0;
}