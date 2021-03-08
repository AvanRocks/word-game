#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <cstring>
#include <algorithm>
using namespace std;

vector<string> words;
map<string,bool> vis;
map<string,int> dis, paths;
map<string,string> last;
map<string, vector<string>> adj;
int wordLen;

bool isWord(string word) {
	auto it = lower_bound(words.begin(),words.end(),word);
	return (*it == word);
}

void initWords() {
	ifstream wordsFile("words", ifstream::in);

	char tmp[100];
	while(wordsFile.getline(tmp,90)) {
		if (strlen(tmp) == wordLen)
			words.push_back(tmp);
	}

	wordsFile.close();
}

void initAdj() {
	for (string curr : words) {
		for (int i=0;i<curr.size();++i) {
			char letter = curr[i]-'a';
			for (int j=0;j<26;++j) {
				if (j==letter) continue;
				string next = curr;
				next[i]=(char)('a'+j);
				if (isWord(next))
					adj[curr].push_back(next);
			}
		}
	}
}

void init() {
	initWords();
	initAdj();
}

int main () {
	string start = "ship";
	string end = "dock";

	cout<<"Enter the starting word: ";
	cin>>start;
	cout<<"Enter the target word: ";
	cin>>end;
	cout<<'\n';

	if (start.size() != end.size()) {
		cout<<"The word lengths don't match\n";
		exit(0);
	}
	wordLen=start.size();

	init();

	queue<string> q;
	q.push(start);
	dis[start]=0;
	vis[start]=1;
	paths[start]=1;
	last[start]="X";
	string curr;

	bool pos=0;
	while(!q.empty()) {
		curr=q.front(); q.pop();
		if (curr==end) {
			pos=1;
			break;
		}
		for (string next : adj[curr]) {
			if (!vis[next]) {
				q.push(next);
				vis[next]=1;
				last[next]=curr;
			}
			if (dis[next]==0 || dis[next]>dis[curr]+1) {
				dis[next]=dis[curr]+1;
				paths[next]=paths[curr];
			} else if (dis[next]==dis[curr]+1) {
				paths[next]+=paths[curr];
			}
		}
	}

	if (pos) {
		cout<<"Number of shortest solutions: "<<paths[end]<<'\n';
		cout<<"Length of shortest solution: "<<dis[end]<<'\n';
		cout<<"One possible solution: \n";
		vector<string> path;
		string curr=end;
		while(curr!="X") {
			path.push_back(curr);
			curr=last[curr];
		}
		reverse(path.begin(),path.end());
		for (string str : path) cout<<str<<'\n';
	} else {
		cout<<"Not possible\n";
	}

	return 0;
}
