#include <iostream>
#include <cstring>
using namespace std;
///*prototypes*///
//main functions
void conversion  (char* , char*);
int  evaluation  (char*);
//stack operations
void push(int);
int  pop();
bool isEmpty();

int priority(char);
//mathematical operations
int addition		(int , int);
int subtraction		(int , int);
int multiplication	(int , int);
int division		(int , int);
int exponentiation	(int , int);
int factorial		(int);
int unaryminus		(int);
//conditions
bool isOperand (char);
bool isOperator(char);
bool isPushable(char);
//*global variables(of stack)*//
int s[10];
int top = -1;

bool nextToParanthesis = false;//to detect unary minus
int variable[2][61];


int main()
{
	char in[101] , post[101];
	cout << "enter infix expression:" ;
	cin  >> in ;
	conversion(in , post);
	evaluation(post);
	return 0;
}
//-----------------------------------------conversion-----------------------------------------
void conversion(char* in , char* post)
{
	int j = 0;
	for (int i = 0; i < strlen(in); i++)//Tokenization
	{	
		if 		(isOperand(in[i]) )
			post[j++] = in[i] ;//if token is operand append it to postfix
   		else if (isOperator(in[i]))
				if 		(in[i] == '(')
				{	
					push(in[i]);//push and type cast(char to int)
					nextToParanthesis = true;continue;
				}
				else if (in[i] == ')')
				{
					while (s[top] != '(')
						post[j++] = pop();//append and type cast(int to char)
					pop();	
			    }
				else if (!isPushable(in[i]))
			    {
					post[j++] = pop();
					if (!isPushable(in[i]))//because of these cases:^ on the */+-  OR  */ on the +- :size of stack (on the most up parenthesis) must be popped = 2
					{	
						post[j++] = pop();
						if (!isPushable(in[i]))//because of this case(worst case):^ on the */ on the +- :size of stack (on the most up parenthesis) must be popped = 3
							post[j++] = pop();
				    }
					push(in[i]);	
			    }
				else
					push(in[i]);
		else
		{
			cerr << "Error!\a\ninvalid character : " << in[i] << "\ntry again\n";
			main(); 
	    }
		nextToParanthesis = false;	
	// cout << top << "," << j << "\n";//barresie nahveye por o khali shodane stack
	}
	// cout << (char*)s << endl;//namyeshe stack baad as khoondane tamame infix
	while( !isEmpty() )	
	{
		if (s[top] == '('){top--;continue;}
    	post[j++] = pop();//append
    }
	post[j] = '\0';
	cout << "the postfix expression:"<< post << endl;
}
//-----------------------------------------stack operations*
void push(int op)//op stands for: operator for operators' stack OR operand for operands' stack
{
	s[++top] = op;
}
int pop()
{
	return s[top--];
}
bool isEmpty()
{
	if(top == -1) return true;
	return false;
}
//-----------------------------------------operators priority*
int priority(char optr)
{
	if(nextToParanthesis && optr == '-')return 4;
	switch(optr)
	{
		case '(': return 0;//to enables us pushing any operater on it
		case '+': return 1;
		case '-': return 1;
		case '*': return 2;
		case '/': return 2;
		case '^': return 3;//because this equation is false (a*b)^c = a*(b^c) or a*b^c = b^c*a
		case '!': return 5;
	}
	return -100000;

}
//-----------------------------------------evaluation-----------------------------------------
int evaluation(char* post)
{
	top = -1;//to make stack empty(instead of using a new stack)
	//	int variable[2][51];
	short counter = 0;//numbers of variables
	bool newvariable;
	for (int i = 0; i < strlen(post); i++)
	{
		if(isOperand(post[i]))
		{
			newvariable = true;
			for (int j = 1; 0 <= i-j; j++)
			{
				if(post[i] == post[i-j]) 
				{
					newvariable = false;
				}
			}
			if(newvariable)			
			{
				cout << "enter the value of " << post[i] << ":";
				cin >> variable[0][counter];
				push(variable[0][counter]);
				variable[1][counter++] = (int)post[i];
			}
			else//if variable was repeated
				for(int i = 0; i < counter; i++){
					if(variable[1][i] == post[i])push(variable[0][i]);
				}
		}
		else
		{
			switch(post[i])//if token is operator
			{
				case '+' : push( addition	    ( pop() , pop() ) );	break;//aval dovomi pop mishe    
				case '-' : push( subtraction	( pop() , pop() ) );	break;
				case '*' : push( multiplication ( pop() , pop() ) );	break;
				case '/' : push( division	    ( pop() , pop() ) );	break;
				case '^' : push( exponentiation ( pop() , pop() ) );	break;
				case '!' : push( factorial    	( pop() ) 		  );    //mishod factorial ro too evalua handel kard
			}
		}
	}
	cout << "the answer is:" << s[top];
	return pop();
}
//-----------------------------------------addition*
int addition(int v1 , int v2)//v stands for variable
{
	return v1 + v2;
}
//-----------------------------------------subtraction*
int subtraction(int v1 , int v2)
{
	return v1 - v2;
}
//-----------------------------------------multiplication*
int multiplication(int v1 , int v2)
{
	return v1 * v2;
}
//-----------------------------------------division*
int division(int v1 , int v2)
{
	return v1 / v2;
}
//-----------------------------------------exponentiation*
int exponentiation(int v1 , int v2)
{
	int temp = 1;
	while(0 < v2--)
		temp *= v1;	
	return temp;
}
//-----------------------------------------factorial*
int factorial(int v)
{
	int temp = 1;
	for (int i = 2; i <= v; i++)
		temp *= i;
	return temp;
}
//-----------------------------------------unary minus*
int unaryminus(int v)
{
	return -v;
}
//-----------------------------------------conditions-----------------------------------------
bool isOperand(char op)//stands for operator OR operand
{
	if (65 <= op && op <= 90 || 97 <= op && op <= 122)//if ('A' <= op && op <= 'Z' || 'a' <= op && op <= 'z')
		return true;//this condision covers whether uppercase or lowercase letters
	return false;
}
bool isOperator(char op)
{
	switch(op)
	{
		case '+': return true;
		case '-': return true;
		case '*': return true;
		case '/': return true;
		case '(': return true;
		case ')': return true;
		case '^': return true;
		case '!': return true;//order based on frequency of them( to minimize run commands?)
		default : return false;
	}
}
bool isPushable(char optr)//stands for operator
{
	if (isEmpty() || priority(s[top]) < priority(optr))
		return true;
	return false;	
}