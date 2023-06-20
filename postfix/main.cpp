#include<string>
#include<iostream>
#include<vector>
#include<set>
#include<stack>
#include <sstream>
#include<string.h>
using namespace std;

enum term_type {
	t_const,	
	t_var,
	t_opbl, //open bracket left
	t_clbl,
	t_opl, //left operation
	t_op, // operation between 2 vars
	t_opr, //operation right
	t_fun,
	t_zero
};

struct term {
	string body;
	term_type type;
};

struct operation {
	string body;
	term_type type;
	int priority;
};
set<char> Oper = { '*','/','-','+','&','|','^','!','%' };
set<char> Opbl = { '<','(','[','{' };
set<char> Clbl = { '>',')',']','}' };

vector<operation> Operations = { 
	{"^",t_op,8},
	{"-",t_opl,8}, 
	{"*",t_op,7},
	{"/",t_op,7},
	{"%",t_op,7},
	{"!",t_opr,8},
	{"+",t_op,5},
	{"-",t_op,5},
	{"&",t_op,4},
	{"|",t_op,4}, 
	{"!",t_opl,8},
	{"||",t_op,3},
	{"&&",t_op,3} 
};

void split(string const& str, const char tmp, vector<std::string>& out)
{
	stringstream ss(str);
	string s;
	while (getline(ss, s, tmp)) {
		out.push_back(s);
	}
}
vector<term> get_term(string expression) {
	vector<term> ans;
	vector<string> body_ans;
	split(expression, ' ', body_ans);//разбиение
	term PartAns;
	term_type Type;
	for (int i = 0; i < body_ans.size(); i++) {
		if (Oper.find(body_ans[i][0]) != Oper.end()) {
			Type = t_op;
		}
		if (isalpha((unsigned char)body_ans[i][0])) {
			Type = t_var;
		}
		if (isdigit(body_ans[i][0])) {
			Type = t_const;
		}
		if (Opbl.find(body_ans[i][0]) != Opbl.end()) {
			if (i > 0 && ans[i - 1].type == t_var) { 
				ans[i - 1].type = t_fun;
			}
			if (i > 0 && ans[i - 1].type == t_op) { 
				ans[i - 1].type = t_opl;
			}
			if (i > 0 && ans[i - 1].type == t_opr) {
				ans[i - 1].type = t_op;
			}
			Type = t_opbl;
		}
		if (Clbl.find(body_ans[i][0]) != Clbl.end()) {
			Type = t_clbl;
		}
		//блок корректировки типа операций
		if (Type == t_op && i > 0) {
			if (ans[i - 1].type == t_var || ans[i - 1].type == t_const || ans[i - 1].type == t_fun || ans[i - 1].type == t_clbl) {
				Type = t_opr;
			}
		}
		if ((Type == t_var || Type == t_const || Type == t_fun) && i > 0) {
			if (ans[i - 1].type == t_op) 
			{
				ans[i - 1].type = t_opl;
			}
			if (ans[i - 1].type == t_opr) 
			{
				ans[i - 1].type = t_op;
			}
		}
		PartAns.body = body_ans[i];
		PartAns.type = Type;
		ans.push_back(PartAns);
	}
	return ans;
}
int get_priority(term a) {
	for (int i = 0; i < Operations.size(); i++) {
		if (a.body == Operations[i].body && a.type == Operations[i].type) 
		{
			return Operations[i].priority;
		}
	}
	return 0;
}
int top_priority(stack<int> a) {
	if (a.empty()) 
	{
		return 0;
	}
	else 
	{
		return a.top();
	}
}
vector<term> postfix_notation(vector<term> Terms) {
	stack<int> SPriority;//priority stack
	stack<term> S;
	vector<term> out;
	for (int i = 0; i < Terms.size(); i++) {
		if (Terms[i].type == t_fun) 
		{
			S.push(Terms[i]);
			SPriority.push(10); // func priority = 10
		}
		if (Terms[i].type == t_opbl) 
		{
			S.push(Terms[i]);
			SPriority.push(0); // open bracket left = 0
		}
		if (Terms[i].type == t_const || Terms[i].type == t_var) 
		{
			out.push_back(Terms[i]); 
		}
		if (Terms[i].type == t_op || Terms[i].type == t_opl || Terms[i].type == t_opr) {
			int Priority = get_priority(Terms[i]);			
			int TopStack = top_priority(SPriority);
			while (Priority <= TopStack) 
			{
				out.push_back(S.top());
				S.pop();
				SPriority.pop();
				TopStack = top_priority(SPriority);
			}
			if (Priority > TopStack) 
			{
				S.push(Terms[i]);
				SPriority.push(Priority);
			}
		}
		if (Terms[i].type == t_clbl) { 
			while (S.top().type != t_opbl) {
				out.push_back(S.top());
				S.pop();
				SPriority.pop();
			}
			S.pop();
			SPriority.pop();
		}
	}
	while (!S.empty()) {
		out.push_back(S.top());
		S.pop();
		SPriority.pop();
	}
	return out;
}
vector<term> change_to_var(vector<term> postf_not) { 
	for (int i = 0; i < postf_not.size(); i++) {
		if (postf_not[i].type == t_var) {
			cout << "Write var " << postf_not[i].body << endl;
			string to_change = postf_not[i].body;
			string edited;
			cin >> edited;
			for (int j = i; j < postf_not.size(); j++) {
				if (postf_not[j].body == to_change) 
				{
					postf_not[j].body = edited;
					postf_not[j].type = t_const;
				}
			}
		}
	}
	return postf_not;
}
float postfix_notation_calc(vector<term> postf_not) {
	stack<float> S;
	for (int i = 0; i < postf_not.size(); i++) {
		if (postf_not[i].type == t_const) { 
			S.push(atof(postf_not[i].body.c_str()));
		}
		if (postf_not[i].type == t_opl) {
			float Top = S.top();
			S.pop();
			if (postf_not[i].body == "!") {
				Top = !Top;
			}
			if (postf_not[i].body == "-") {
				Top = -Top;
			}
			S.push(Top);
		}
		if (postf_not[i].type == t_opr) {
			float Top = S.top();
			S.pop();
			if (postf_not[i].body == "!") {
				int mul = 1;
				for (int i = 2; i < Top; i++) {
					mul *= i;
				}
				Top = mul;
			}
			S.push(Top);
		}
		if (postf_not[i].type == t_op) {
			float Top = S.top();
			S.pop();
			float Top2 = S.top();
			S.pop();
			float Ans = 0;
			if (postf_not[i].body == "+") {
				Ans = Top2 + Top;
			}
			if (postf_not[i].body == "-") {
				Ans = Top2 - Top;
			}
			if (postf_not[i].body == "*") {
				Ans = Top2 * Top;
			}
			if (postf_not[i].body == "/") {
				Ans = Top2 / Top;
			}
			if (postf_not[i].body == "^") {
				Ans = pow(Top2, Top);
			}
			if (postf_not[i].body == "%") {
				Ans = (int)Top2 % (int)Top;
			}
			if (postf_not[i].body == "|") {
				Ans = (int)Top2 | (int)Top;
			}
			if (postf_not[i].body == "&") {
				Ans = (int)Top2 & (int)Top;
			}
			if (postf_not[i].body == "||") {
				Ans = (int)Top2 || (int)Top;
			}
			if (postf_not[i].body == "&&") {
				Ans = (int)Top2 && (int)Top;
			}
			S.push(Ans);
		}
		if (postf_not[i].type == t_fun) { //если функция
			if (postf_not[i].body == "sin") {
				float Top = S.top();
				S.pop();
				S.push(sin(Top));
			}
			if (postf_not[i].body == "cos") {
				float Top = S.top();
				S.pop();
				S.push(cos(Top));
			}
			if (postf_not[i].body == "tan") {
				float Top = S.top();
				S.pop();
				S.push(tan(Top));
			}
			if (postf_not[i].body == "exp") {
				float Top = S.top();
				S.pop();
				S.push(exp(Top));
			}
		}
	}
	return S.top();
}
int main()
{
	string s1 = "( abc + 123 ) / 2";
	vector<term> Ans = get_term(s1);
	for (term& t : Ans) {
		cout << t.body << " " << t.type << endl;
	}
	cout << endl;
	Ans = postfix_notation(Ans);
	for (term& t : Ans) {
		cout << t.body << " ";
	}
	cout << endl;
	Ans = change_to_var(Ans);
	for (term& t : Ans) {
		cout << t.body << " ";
	}
	float Answer = postfix_notation_calc(Ans);
	cout << endl << (float)Answer;
	return 0;
}