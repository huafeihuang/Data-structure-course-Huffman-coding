//haffmantree 
//功能
//1，读入文件，生成编码，（文件内容生成频率构建haffman）
//2，保存编码到文本
//3，读入文件解码
//4，保存解码到文本
//注意，选择的初始文本字符不能少于两类
#define SHOW_CONSOLE
#include<fstream>
#include<cstdio>
#include<iostream>
#include<string>
#include<graphics.h>
#include<cmath>
#define MAXSIZE 200 
using namespace std;
typedef string *Hcode;
typedef struct {
	char c;
	int weight;
	int lch, rch, parent;
}node;
typedef node HuffmanTree[MAXSIZE];
/*  界面功能循环：1，试验 2，编码（输入文件，编码后输出并保存） 3解码（输入文件，解码后输出并保存） 4重构haffman（重新输入权重txt）5退出*/
//初始化haffman树
int Find_Root(HuffmanTree H, int r) {
	if (abs(H[1].weight)<2000) {
		if (r != -1) {
			if (H[r].parent != -1) {
				return Find_Root(H, H[r].parent);
			}
			else {
				return r;
			}
		}
		else return -1;
	}
	else {
		cerr << "未初始化哈弗曼编码（请先执行 ①选项）" << endl;
	}
	return -1;
}
int Not_duplicated(string a, char b, int n) { //原始字符串，比较字符，字符数组大小
	const char*str = a.c_str();
	int flag = 1;//返回101为没有重复
	for (int i = 0; i < n; i++) {
		if (b == a[i]) {
			flag = 0;
			break;
		}
	}
	return flag;
}
void StrCount(int* n, string *a, string* d, int*c) { //不重复个数，原始串，不重复字符串，不重复统计
	if (a->length()>0) {
		int nn = (int)(*a).length();
		int co = 0;
		for (int i = 0; i < nn; i++) {
			if (Not_duplicated((*a), (*a)[i], i)) {
				c[co]++; co++;
				*d = *d + (*a)[i];
			}
			else {
				for (int j = 0; j <= co; j++) {
					if ((*a)[i] == (*d)[j]) {
						c[j] ++; break;
					}
				}
			}
		}
		*n = co;
	}
	else {
		printf("\t\t╔══════════════════╗\n");
		printf("\t\t║             无字符输入             ║\n");
		printf("\t\t╚══════════════════╝\n\n");
	}

}
void ReadFile(string ffname, string *a, int* n, string *drop, int *c) {//文件名，原始字符串，不重复个数，不重复字符串，不重复统计
	string fname = ffname;
	string str = "";
	string buffer;
	ifstream in;
	in.open(fname.c_str(), ios::out);
	if (in.is_open()) {
		while (getline(in, buffer)) {
			str = str + buffer;
		}
	}
	else {
		printf("\t\t╔══════════════════╗\n");
		printf("\t\t║           未成功打开文件           ║\n");
		printf("\t\t╚══════════════════╝\n\n");
		return;
	}
	in.close();
	//统计
	for (int i = 0; i < MAXSIZE; i++)c[i] = 0;
	StrCount(n, &str, drop, c);
	*a = str;//原始字符串
};
void InitialTree(HuffmanTree H, int n) {//哈弗曼树，不重复个数
	if (n>0) {
		for (int i = 0; i < MAXSIZE; i++) {
			H[i].c = ' ';
			H[i].weight = 0;
			H[i].lch = -1;
			H[i].rch = -1;
			H[i].parent = -1;
		}
		printf("\t\t╔══════════════════╗\n");
		printf("\t\t║        成功初始化哈弗曼树          ║\n");
		printf("\t\t╚══════════════════╝\n\n");
	}
	else {
		printf("\t\t╔══════════════════╗\n");
		printf("\t\t║       未成功初始化哈弗曼树         ║\n");
		printf("\t\t╚══════════════════╝\n\n");
		return;
	}
};
void InputWeight(HuffmanTree H, string *se, int*c, int n) {//不重复字符串，不重复统计，不重复个数
	if (n>0)
	{
		const char* cc = (*se).c_str();
		for (int i = 0; i < n; i++) {
			H[i].c = cc[i];
			H[i].weight = c[i];
		}
		printf("\t\t╔══════════════════╗\n");
		printf("\t\t║            成功输入权重            ║\n");
		printf("\t\t╚══════════════════╝\n\n");
	}
	else {
		printf("\t\t╔══════════════════╗\n");
		printf("\t\t║           输入权重失败             ║\n");
		printf("\t\t╚══════════════════╝\n\n");
	}
};//输入权重
void SelectTwoMin(HuffmanTree HT, int end, int *s1, int *s2) {//哈弗曼树，最后值，最小一，最小二
	int min1 = 999999;
	int min2 = 999999;
	int i, j;
	for (i = 0; i < end; i++) {
		if (HT[i].parent == -1)//根节点
			if (HT[i].weight <= min1) {
				min1 = HT[i].weight;
				*s1 = i;
			}
	}
	for (j = 0; j < end; j++) {
		if (HT[j].parent == -1)//根节点
			if (HT[j].weight <= min2 && (*s1) != j) {
				min2 = HT[j].weight;
				*s2 = j;
			}
	}
};//选择两个最小的节点
void CreatTree(HuffmanTree H, string fname, int *nn) {
	int n = 0;//不重复字符串字数
	string first;//原始读入字符串
	string nodustr = "";//不重复字符串
	int count[MAXSIZE];//不重复字符统计
	ReadFile(fname, &first, &n, &nodustr, count);
	InitialTree(H, n);
	InputWeight(H, &nodustr, count, n);
	*nn = n;
	int p1 = -1, p2 = -1;
	for (int i = n; i < 2 * n - 1; i++) {//最底层父节点构造
		SelectTwoMin(H, i, &p1, &p2);
		H[p1].parent = i;
		H[p2].parent = i;
		H[i].lch = p1;
		H[i].rch = p2;
		H[i].weight = H[p1].weight + H[p2].weight;
	}
};//包含上面三个函数,创造出一颗完整的树
void Print(HuffmanTree H, int x, int y, int r, int boo, int xlen, int ylen, int sub, int subb) {//树，根节点x，y,根节点，左右，x长，y长，减数
	if (H[r].weight >= -1) {
		Print(H, x - xlen, y + ylen, H[r].lch, 0, xlen - (sub - subb), ylen, (sub - subb), subb);//left
		fillellipse(x, y, 15, 15);
		if (H[r].c != ' ') {
			outtextxy(x - 5, y - 5, H[r].c);
		}
		if (r != Find_Root(H, r)) {
			line(x, y, boo == 0 ? (x + xlen + sub) : (x - xlen - sub), y - ylen);
			outtextxy(boo == 0 ? (x + (xlen + sub) / 2) : (x - (xlen + sub) / 2), y - (ylen) / 2, char('0' + boo));
		}
		Print(H, x + xlen, y + ylen, H[r].rch, 1, xlen - (sub - subb), ylen, (sub - subb), subb);//right
	}
	else return;
}
void Struct_print(HuffmanTree H, int x, int y, int r, int n) {//哈弗曼树，根x,根y，根，数目
	if (n>0) {
		initgraph(1500, 600);
		outtextxy(20, 20, "该编码的哈弗曼树：");
		outtextxy(20, 40, "（按任意键退出）");
		outtextxy(20, 60, "树形结构");
		setcolor(EGERGB(248, 248, 255));
		//setbkcolor(EGERGB(0x0, 0x40, 0x0));//(0x0, 0x0, 0x40)浅蓝，(0xFF, 0xFF, 0x0)黄，(0x0, 0x40, 0x0)绿，(0xFF, 0x0, 0x80)粉 0x80, 0x0, 0xFF紫 (0xFF, 0x0, 0x0)红
		setfillcolor(EGERGB(0xFF, 0x0, 0x0));
		setfontbkcolor(EGERGB(0x0, 0x40, 0x0));
		Print(H, x, y, r, 0, 170, 50, 32, 0);
		getch();
		closegraph();
	}
	else {
		cerr << "未创建哈弗曼编码（请先执行 ①选项）" << endl;
		printf("\t\t╔══════════════════╗\n");
		printf("\t\t║            按任意键继续            ║\n");
		printf("\t\t╚══════════════════╝\n");
		getchar();
	}
}
Hcode HcGeneration(HuffmanTree H, Hcode hc, int n) {//生成建树的哈弗曼编码
	if (n>0) {
		hc = new string[n];
		int p, c, i;
		for (i = 0; i < n; i++) {
			for (c = i, p = H[i].parent; p != -1; c = p, p = H[p].parent) {
				if (H[p].lch == c) {//左孩子
					hc[i] = "0" + hc[i];
				}
				else {
					hc[i] = "1" + hc[i];
				}
			}
		}
		return hc;
	}
	else {
		printf("\t\t╔══════════════════╗\n");
		printf("\t\t║         哈弗曼编码创建失败         ║\n");
		printf("\t\t╚══════════════════╝\n");
		return NULL;
	}
};//生成哈弗曼树的编码
void Encode(string Original, string CodeFile, Hcode hc, HuffmanTree HT, int n) {//原字符文件名，编码后文件名，编码表，哈弗曼树,字数
	if (Original.length()>0 && CodeFile.length()>0 && n>0) {
		string str = "";//读入的原始编码
		string Hfcode = "";
		string buffer;
		int len;
		ifstream in;
		in.open(Original.c_str(), ios::in | ios::out);
		if (in.is_open()) {
			while (getline(in, buffer)) {//读入
				str = str + buffer;
			}
			in.close();
			len = str.length();
			printf("\t╔══════════════════════╗\n");
			cout << "\t\t原文内容为：\n\t\t" << str << "\n\t\t" << "长度为：" << len << endl;
			printf("\t╚══════════════════════╝\n");
			const char* ch = str.c_str();
			int k = 0, i;
			while (ch[k] != '\0') {
				for (i = 0; i < len; i++) {
					if (ch[k] == HT[i].c) {
						Hfcode += hc[i];
						break;
					}
				}
				k++;
			}
			printf("");
			printf("\t╔══════════════════════╗\n");
			cout << "\t\t现已编码为：\n" << "\t\t" << Hfcode << "\n\t\t" << "长度为：" << Hfcode.length() << endl;
			printf("\t╚══════════════════════╝\n\n\n");
			ofstream out;
			out.open(CodeFile.c_str(), ios::out);
			if (out.is_open()) {
				out << Hfcode.c_str() << endl;
				out.close();
				printf("\t╔══════════════════════╗\n");
				cout << "\t\t已成功将编码的文件内容写入文件：" << CodeFile.c_str() << endl;
				printf("\t╚══════════════════════╝\n\n\n");
			}
			else {
				printf("\t\t╔══════════════════╗\n");
				printf("\t\t║           未成功写入文件           ║\n");
				printf("\t\t╚══════════════════╝\n\n");
			}
		}
		else {
			printf("\t\t╔══════════════════╗\n");
			printf("\t\t║           未成功打开文件           ║\n");
			printf("\t\t╚══════════════════╝\n\n");
		}
	}
	else {
		printf("\t\t╔══════════════════╗\n");
		printf("\t\t║    文件名没有值或没有哈弗曼编码！  ║\n");
		printf("\t\t╚══════════════════╝\n\n");
	}
};//利用树进行编码
void Decode(string CodeFile, string TextFile, HuffmanTree HT, int n) {//编码文件名，解码文件名，哈弗曼树，不重复字符数
	if (CodeFile.length() > 0 && TextFile.length() > 0 && n>0) {
		char ch = ' ';
		string str = "";//要解码的字符
		string Hfcode = "";//读入的哈弗曼编码
		string buffer;
		int len, i = 0;
		int j = Find_Root(HT, 1);//2 * n - 1 - 1;//从根节点向下搜索
		ifstream in;
		in.open(CodeFile.c_str(), ios::in | ios::out);
		if (in.is_open()) {
			while (getline(in, buffer)) {//读入
				Hfcode = Hfcode + buffer;
			}
			in.close();
			len = Hfcode.length();
			printf("\t╔══════════════════════╗\n");
			cout << "\t\t原哈弗曼编码为：\n\t\t" << Hfcode << "\n\t\t" << "长度：" << len << endl;
			printf("\t╚══════════════════════╝\n");
			if (HT == NULL) {
				printf("\t\t╔══════════════════╗\n");
				printf("\t\t║           请先编码（操作②）       ║\n");
				printf("\t\t╚══════════════════╝\n\n");
				return;
			}
			const char* code = Hfcode.c_str();
			while (code[i] != '\0') {
				if (code[i] == '0') {//0
					j = HT[j].lch;
				}
				else {//1
					j = HT[j].rch;
				}
				if (HT[j].rch == -1&&HT[j].lch == -1) {
					ch = HT[j].c;
					str += ch;
					/*j = n * 2 - 1 - 1;*/
					j = Find_Root(HT, 1);
				}
				i++;
			}
			//开始写入
			printf("\t╔══════════════════════╗\n");
			cout << "\t\t现已解码为：\n" << "\t\t" << str << "\n\t\t长度为：" << str.length() << endl;
			printf("\t╚══════════════════════╝\n\n\n");
			ofstream out;
			out.open(TextFile.c_str(), ios::out);
			if (out.is_open()) {
				out << str.c_str() << endl;
				out.close();
				printf("\t╔══════════════════════╗\n");
				cout << "\t\t已成功将解码文件内容写入文件：" << TextFile.c_str() << endl;
				printf("\t╚══════════════════════╝\n\n\n");
			}
			else {
				printf("\t\t╔══════════════════╗\n");
				printf("\t\t║           未成功写入文件           ║\n");
				printf("\t\t╚══════════════════╝\n\n");
			}
		}
		else {
			printf("\t\t╔══════════════════╗\n");
			printf("\t\t║           未成功打开文件           ║\n");
			printf("\t\t╚══════════════════╝\n\n");
		}
	}
	else {
		printf("\t\t╔══════════════════╗\n");
		printf("\t\t║   文件名没有值或没有哈弗曼编码！   ║\n");
		printf("\t\t╚══════════════════╝\n\n");
	}
};//利用树进行解码
void PrintHfcode(HuffmanTree H, Hcode hc, int n) {//哈弗曼编码表，不重复字符数
	if (n>0) {
		printf("\t\t文本字符对应的哈弗曼编码如下：\n");
		printf("\t\t哈弗曼的编码个数： %d\n", n);
		for (int i = 0; i < n; i++) {
			cout << "\t\t" << H[i].c << "  :    " << hc[i] << endl;
		}
	}
	else { cout << "未成功输出" << endl; }
}
int StartUI() {
	getchar();
	system("cls");
	printf("\t\t╪╪╪╪╪╪╧╧╧╧╧╧╧╧╧╧╪╪╪╪╪╪\n");
	printf("\t\t╔═══╧╧ 哈弗曼编码译码程序 ╧╧═══╗\n");
	printf("\t\t║\t※①.读取并解析文件 \t\t  ║\n");
	printf("\t\t║\t※②.哈弗曼编码保存 \t\t  ║\n");
	printf("\t\t║\t※③.哈弗曼译码保存 \t\t  ║\n");
	printf("\t\t║\t※④.打印哈弗曼树   \t\t  ║\n");
	printf("\t\t║\t※⑤.显示程序状态   \t\t  ║\n");
	printf("\t\t║\t※⑥.退出系统       \t\t  ║\n");
	printf("\t\t╚════════════════════╝\n\n\n");
	printf("\t\t╔═════════════════════╗\n");
	printf("\t\t║═══════输入你的选项════════║:\n");
	printf("\t\t╚═════════════════════╝\n>>> ");
	int c;
	cin >> c;
	return c;
};
void Status(HuffmanTree H, Hcode HC, int n, string *A, string *B, string *C, int *HH, int *HCC, int *NN, int*a, int*b, int*c) {
	//哈弗曼树，哈弗曼编码，字数，原名，编名，译名，六个分别对应参数
	int i, flag = 1;
	for (i = 0;i < 2;i++) {
		if (abs(H[i].weight) > 5000) {
			flag = 0;break;
		}
	}
	*HH = flag;flag = 1;//哈弗曼编码
	if (NULL == HC) {
		flag = 0;;
	}
	*HCC = flag;flag = 1;//哈弗曼编码
	if (n <= 0) {
		flag = 0;
	}*NN = flag;flag = 1;//字符数
	if ((*A).length() == 0)(*a) = 0;
	if ((*B).length() == 0)(*b) = 0;
	if ((*C).length() == 0)(*c) = 0;
};
string Panduan(int a) {
	if (a)return "yes";
	else return "no ";
}
void main() {//PrintTree有错
	system("color 3F");
	//system("mode con cols=80 lines=40");
	int n = 0;
	int flag = 1;
	Hcode hc = NULL;//哈弗曼编码
	string frname = "";//原文件名
	string foname = "";//编码文件名
	string fyname = "";//译码文件名
	HuffmanTree H;//node数组
	printf("\t   ╪╪╪╪╪╪╪╪╧╧╧╧╧╧╧╧╧╧╪╪╪╪╪╪╪╪\n");
	printf("\t   ╔════╧╧欢迎使用哈弗曼编码译码器╧╧════╗\n");
	printf("\t   ╚════════════════════════╝\n\n\n");
	printf("\t\t╔══════════════════╗\n");
	printf("\t\t║            按回车键继续            ║\n");
	printf("\t\t╚══════════════════╝\n");
	while (flag) {
		switch (StartUI()) {
		case 1: {
			printf("\t该目录下的所有txt文件如下所示：\n");
			printf("\t╔══════════════════════╗\n");
			system("dir *.txt");
			printf("\t╚══════════════════════╝\n");
			printf("\n\t现在请选择你想操作的文件名（txt文件）:\n");
			printf("\t═══════════════════════\n>>> ");
			cin >> frname;
			CreatTree(H, frname.c_str(), &n);
			hc = HcGeneration(H, hc, n);
			printf("\t\t╔══════════════════╗\n");
			printf("\t\t║            按回车键继续            ║\n");
			printf("\t\t╚══════════════════╝\n");
			getchar();
		} break;
		case 2: {
			printf("\t请输入你想保存编码的文件名\n");
			printf("\t═══════════════════════\n>>>");
			cin >> foname;
			printf("\t═══════════════════════\n");
			Encode(frname.c_str(), foname.c_str(), hc, H, n);
			printf("\t\t╔══════════════════╗\n");
			printf("\t\t║            按回车键继续            ║\n");
			printf("\t\t╚══════════════════╝\n");getchar(); }break;
		case 3: {
			printf("\t请输入你想保存译码的文件名\n");
			printf("\t═══════════════════════\n>>>");
			cin >> fyname;
			printf("\t═══════════════════════\n");
			Decode(foname.c_str(), fyname.c_str(), H, n);
			printf("\t\t╔══════════════════╗\n");
			printf("\t\t║            按回车键继续            ║\n");
			printf("\t\t╚══════════════════╝\n");getchar(); }  break;
		case 4:Struct_print(H, 600, 50, Find_Root(H, 0), n);  break;
		case 5: {
			int HH = 1, HCC = 1, NN = 1, RR = 1, OO = 1, YY = 1;
			Status(H, hc, n, &frname, &foname, &fyname, &HH, &HCC, &NN, &RR, &OO, &YY);
			printf("\t\t哈弗曼编码/译码系统信息：\n");
			printf("\t\t╔══════════════╗\n");
			printf("\t\t║══════════════║\n");
			printf("\t\t║哈弗曼树是否建立：  |    ");cout << Panduan(HH) << "║" << endl;
			printf("\t\t║哈弗曼编码是否建立：|    ");cout << Panduan(HCC) << "║" << endl;
			printf("\t\t║原文本字符个数建立：|    ");cout << Panduan(NN) << "║" << endl;
			printf("\t\t║已尝试解析原文内容：|    ");cout << Panduan(RR) << "║" << endl;
			printf("\t\t║已尝试执行编码操作：|    ");cout << Panduan(OO) << "║" << endl;
			printf("\t\t║已尝试执行解码操作：|    ");cout << Panduan(YY) << "║" << endl;
			printf("\t\t║══════════════║\n");
			printf("\t\t╚══════════════╝\n");
			if (HH&&HCC&NN)PrintHfcode(H, hc, n);
			printf("\t\t╔══════════════════╗\n");
			printf("\t\t║            按回车键继续            ║\n");
			printf("\t\t╚══════════════════╝\n");
			getchar();
		}  break;
		case 6: {
			printf("\t\t╔══════════════════╗\n");
			printf("\t\t║               已退出               ║\n");
			printf("\t\t╚══════════════════╝\n");flag = 0; } break;
		default: {if (cin.fail()) {
			cin.clear();cin.ignore();
		}
				 printf("\t\t╔══════════════════╗\n");
				 printf("\t\t║  非法选项!请重新选择!(按回车继续)  ║\n");
				 printf("\t\t╚══════════════════╝\n");
				 getchar();
		}break;
		}
	}
	system("pause");
}