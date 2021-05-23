#include <bits/stdc++.h>
#include <chrono>
using namespace std;
#define ll long long
#define pb push_back
#define mp make_pair
#define endl "\n"
#define INF 999999
class fibNode{
public:
	int dis;			//stores distance from source
	int node;			//stores node number
	int degree;
	fibNode* parent;	
	fibNode* right;
	fibNode* left;
	fibNode* leftchild;
	int mark;			//number of times a child has been removed
};
int minfib;
class binNode{
public:
	int dis;
	int node;
	int degree;
	binNode* parent;
	binNode* leftchild;
	binNode* rightsib;
};
//function declarations
void bellmanFord(int n,vector<vector <int> > adjlist,vector<vector <int> > adj,int *flag,vector<int> &dist);
void dijkstra(int n,int s,vector<vector <int> > adjlist,vector<vector <int> > adj,char ch,vector<int> &dist);
void createHeap(vector<pair<int,int> > &binheap,int n,int s,int positions[]);
void dec_key_binary(int x,vector<pair<int,int> > &binheap,int newdist,int positions[]);
void dec_key_binomial(int x,int newdist,vector<binNode*> &pos_bin);
void dec_key_fibonacci(int x,int newdist,vector<fibNode*> &pos_fib,list<fibNode*> &fibonacci);
pair<int,int> extractMin_binary(vector<pair<int,int> > &binHeap,int positions[]);
pair<int,int> extractMin_binomial(vector<binNode*> &pos_bin, list<binNode*> &binomial);
pair<int,int> extractMin_fibonacci(vector<fibNode*> &pos_fib, list<fibNode*> &fibonacci);
fibNode* newNode_fib(int node1,int dist);
binNode* newNode_bin(int node1,int dist);
list<binNode*> union_binomial(list<binNode*> l1,list<binNode*> l2,int y);
int count2=0;
void solve(char ch)
{  
	ll n;
    int d,i,j;
    cin>>n>>d;
    vector< vector<int> > adj(n, vector<int>(n,INF));
	if(n==1)			//base case
	{
		int temp;cin>>temp;
		cout<<"0"<<endl;
		return;
	}
	int flag2=0;				//indicates the presence of a -ve edge weight
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            cin>>adj[i][j];			
            if(i==j)
                adj[i][j]=INF;          //no self loops
			if(adj[i][j]<0)
				flag2=1;
        }
    }
    vector<vector <int> > adjlist(n);		//make adjacency list out of the matrix, to reduce complexity
	 	
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
		    if(adj[i][j]!=INF )				
				adjlist[i].pb(j+1);			//adjlist only contains the node, the edge weight is in the adjacency matrix
		}
	} 
	vector<int> h(n+1,INF);
	if(flag2==1)			//run Bellman Ford to remove -ve edge weights
	{	
		int flag=1;
   	 	bellmanFord(n,adjlist,adj,&flag,h);   
			
		if(flag==0)			//negative cycle exists
		{
			return;
		}
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				if(adj[i][j]!=INF)
				{
					adj[i][j]+=(h[i]-h[j]);
				}				
			}			
		}
	}	
	vector<vector<int> > apsp;		//all pairs shortest paths
	vector<int> dist(n,INF);
	
	for(i=0;i<n;i++)	
	{					
		dijkstra(n,i+1,adjlist,adj,ch,dist);			
		apsp.pb(dist);		
		for(j=0;j<n;j++)
			dist[j]=INF;			
	}		
	for(i=0;i<n;i++)				//print the all pairs shortest paths for the graph
	{			
		for(j=0;j<n;j++)
		{
			if(flag2==1 && apsp[i][j]!=INF)
				cout<<apsp[i][j]-(h[i]-h[j])<<" ";
			else		
				cout<<apsp[i][j]<<" ";
		}
		cout<<endl;
	}
}
int main(int argc, char** argv)	
{
	ios_base:: sync_with_stdio(false);
	cin.tie(0);cout.tie(0);
	int t;        
    cin>>t;  	
    long double time[t];
	int temp=t;
    int i=0;	
	while(t-- )
	{	
		chrono::system_clock::time_point start = chrono::high_resolution_clock::now();
		//runs johnson's algorithm	
		if(argc==1)
			solve('4');			//default=fibonacci
		else
			solve(argv[1][0]);					
		chrono::system_clock::time_point end = chrono::high_resolution_clock::now();
		time[i]=(chrono::duration_cast<chrono::nanoseconds>(end - start).count());
        i++;
	}
	for(i=0;i<temp;i++)
		cout<<time[i]<<" ";
	cout<<endl;
}
void bellmanFord(int n,vector<vector <int> > adjlist,vector<vector <int> > adj,int *flag,vector<int> &dist)
{	
	int i,j;
	vector<int> s1,s2;	
	for(i=0;i<n;i++)
	{
		s1.pb(i+1);				//adjacency list of the new node
		s2.pb(0);				//adjacency matrix of the new node
	}
	s2.pb(INF);				//no self loop
	adjlist.pb(s1);
	adj.pb(s2);	
	int s=n+1;					//new node, which will be the source for bellman ford	
	
	dist[s-1]=0;
	ll edges=0;
	ll relax=0;					//to check number of relaxations; used for finding negative cycle
	for(i=0;i<n+1;i++)
	{
		edges+=adjlist[i].size();
	}
	int flag_1=1;				//to check for negative cycles
	ll act_rel;
	while(relax<=edges*(n+1))
	{		
		act_rel=0;
		for(int x=1;x<=n+1;x++)
		{
			for(i=0;i<adjlist[x-1].size();i++)
			{
				int v=adjlist[x-1][i];
				relax++;					
				if(dist[v-1]>dist[x-1]+adj[x-1][v-1])
				{
					dist[v-1]=dist[x-1]+adj[x-1][v-1];
					act_rel++;					
				}
			}
		}
		if(act_rel==0)
		{
			flag_1=0;
			break;
		}		
	}
	if(flag_1==1)			//negative cycle exists
	{
		cout<<-1<<endl;
		*flag=0;		
	}	
	return;
}
void dijkstra(int n,int s,vector<vector <int> > adjlist,vector<vector <int> > adj,char ch,vector<int> &dist)
{	
	int i,j;	
	dist[s-1]=0;
	int arr[n];	
	vector<pair<int,int> > binheap;
	int positions[n];		//stores the position of the nodes in the array implementation of binary heap
	createHeap(binheap,n,s,positions);				//creates a binary heap with all distances as INF. source is not part of heap
	
	for(i=0;i<n;i++)
	{
		arr[i]=INF;			//initializing array and binary heap 
	}
	arr[s-1]=0;
	list<fibNode*> fibonacci;		//the list of root nodes of fibonacci heap
	list<binNode*> binomial;			//the list of root nodes of binomial heap	
	vector<fibNode*> pos_fib(n,NULL);	//stores the reference to all the nodes; used to reduce complexity of searching in fibonacci heap
	vector<binNode*> pos_bin(n,NULL);	//stores the reference to all the nodes; used to reduce complexity of searching in binomial heap
	
	if(s!=1)
		minfib=1;			//arbitrary. Every node is at INF distance at the beginning
	else 
		minfib=2;
	for(i=0;i<n;i++)			//loop to initialize binomial and fibonacci heaps
	{
		if(i+1!=s)
		{
			//making binomial heap
			list<binNode*> l1;
			binNode* temp1=newNode_bin(i+1,INF);
			pos_bin[i]=temp1;
			l1.pb(temp1);
			binomial=union_binomial(l1,binomial,-1);			//calling union of the new node and the earlier heap

			//making fibonaci heap
			fibNode* temp2=newNode_fib(i+1,INF);
			pos_fib[i]=temp2;
			if(fibonacci.size()!=0)
			{
				(fibonacci.back())->right=temp2;
				temp2->left=fibonacci.back();
			}
			fibonacci.pb(temp2);
		}
	}		
	int count=0;			//stores the number of processed nodes
	count=1;				//source has been processed
	vector<int> processed(n,0);		//stores whether the shortest distance of the node has been calculated or not
	processed[s-1]=1;
	for(i=0;i<adjlist[s-1].size();i++)				//relax the edges from the source node
	{
		int x=adjlist[s-1][i];		
		if(dist[x-1]>dist[s-1]+adj[s-1][x-1])		//relaxation condition
		{
			if(ch=='1')								//array based
			{
				dist[x-1]=dist[s-1]+adj[s-1][x-1];
				arr[x-1]=dist[s-1]+adj[s-1][x-1];
			}
			else if(ch=='2')						//binary heap
			{
				dec_key_binary(x,binheap,dist[s-1]+adj[s-1][x-1],positions);
				dist[x-1]=dist[s-1]+adj[s-1][x-1];
			}
			else if(ch=='3')						//binomial heap
			{
				dec_key_binomial(x,dist[s-1]+adj[s-1][x-1],pos_bin);
				dist[x-1]=dist[s-1]+adj[s-1][x-1];
			}
			else									//fibonacci heap
			{
				dec_key_fibonacci(x,dist[s-1]+adj[s-1][x-1],pos_fib,fibonacci);
				dist[x-1]=dist[s-1]+adj[s-1][x-1];
				
			}
		}
	}			
	while(count<n)
	{		
		int min=INF,minNode=-1;
		if(ch=='1')				//array based implementation
		{
			for(i=0;i<n;i++)			// loop for extracting min in array
			{
				if(arr[i]<min && processed[i]==0)
				{
					min=arr[i];
					minNode=i+1;
				}
			}
			if(min==INF)		//remaining nodes are unreachable
			{
				count=n;
				break;
			}
			dist[minNode-1]=arr[minNode-1];			//final(shortest) distance from the source			
			for(i=0;i<adjlist[minNode-1].size();i++)		//relax edges going out from minNode
			{
				int x=adjlist[minNode-1][i];
				if(dist[x-1]>dist[minNode-1]+adj[minNode-1][x-1] && processed[x-1]==0)
				{
					dist[x-1]=dist[minNode-1]+adj[minNode-1][x-1];
					arr[x-1]=dist[minNode-1]+adj[minNode-1][x-1];
				}
			}
		}
		else if(ch=='2')		//binary heap
		{
			pair<int,int> pi;
			pi=extractMin_binary(binheap,positions);			
			min=pi.first;
			if(min==INF)			//indicated that the remaining nodes are unreachable
			{
				count=n;
				break;
			}
			minNode=pi.second;			
			dist[minNode-1]=min;			//final(shortest) distance from the source	
			for(i=0;i<adjlist[minNode-1].size();i++)		//relax edges going out from minNode
			{
				int x=adjlist[minNode-1][i];
				if(dist[x-1]>dist[minNode-1]+adj[minNode-1][x-1] && processed[x-1]==0)
				{					
					dec_key_binary(x,binheap,dist[minNode-1]+adj[minNode-1][x-1],positions);
					dist[x-1]=dist[minNode-1]+adj[minNode-1][x-1];										
				}
			}
		}
		else if(ch=='3')		//binomial heap	
		{
			pair<int,int> pi;
			pi=extractMin_binomial(pos_bin,binomial);
			min=pi.first;
			if(min==INF)
			{
				count=n;
				break;
			}
			minNode=pi.second;
			dist[minNode-1]=min;			//final(shortest) distance from the source	
			for(i=0;i<adjlist[minNode-1].size();i++)		//relax edges going out from minNode
			{
				int x=adjlist[minNode-1][i];
				if(dist[x-1]>dist[minNode-1]+adj[minNode-1][x-1] && processed[x-1]==0)
				{
					dec_key_binomial(x,dist[minNode-1]+adj[minNode-1][x-1],pos_bin);
					dist[x-1]=dist[minNode-1]+adj[minNode-1][x-1];					
				}
			}
		}
		else					//fibonacci heap
		{
			pair<int,int> pi;
			pi=extractMin_fibonacci(pos_fib,fibonacci);		
			min=pi.first;
			if(min==INF)
			{
				count=n;
				break;
			}
			minNode=pi.second;
			dist[minNode-1]=min;			//final(shortest) distance from the source	
			for(i=0;i<adjlist[minNode-1].size();i++)		//relax edges going out from minNode
			{
				int x=adjlist[minNode-1][i];
				if(dist[x-1]>dist[minNode-1]+adj[minNode-1][x-1] && processed[x-1]==0)
				{
					dec_key_fibonacci(x,dist[minNode-1]+adj[minNode-1][x-1],pos_fib,fibonacci);
					dist[x-1]=dist[minNode-1]+adj[minNode-1][x-1];					
				}
			}
		}
		if(minNode!=-1)
			processed[minNode-1]=1;
		count++;
	}			
	
		for(i=0;i<n;i++)		//freeing up space of binomial heap
		{
			if(pos_bin[i]!=NULL)
			{
				(pos_bin[i])->rightsib=NULL;
				(pos_bin[i])->leftchild=NULL;
				(pos_bin[i])->parent=NULL;				
				delete pos_bin[i];
			}				
		}
		for(i=0;i<n;i++)		//freeing up space of fibonacci heap
		{
			if(pos_fib[i]!=NULL)
			{
				(pos_fib[i])->right=NULL;
				(pos_fib[i])->leftchild=NULL;
				(pos_fib[i])->left=NULL;
				(pos_fib[i])->parent=NULL;
				delete pos_fib[i];
			}
				
		}
}
void createHeap(vector<pair<int,int> > &binheap,int n,int s,int positions[])
{
	int j=0;
	for(int i=0;i<n;i++)
	{	
		if(i!=s-1)			//don't put source in heap
		{
			binheap.pb(mp(INF,i+1));
			positions[i]=j;
			j++;
		}
			
	}	
}
void dec_key_binary(int x,vector<pair<int,int> > &binheap,int newdist,int positions[])
{
	int index=positions[x-1];	
	binheap[index].first=newdist;	
	//performing heapify of the node after deceasing key
	int root=(index-1)/2;
	while(root>=0 && binheap[root].first>binheap[index].first)
	{
		//swap
			positions[binheap[root].second-1]=index;
			positions[binheap[index].second-1]=root;
			pair<int,int> temp=binheap[root];
			binheap[root]=binheap[index];
			binheap[index]=temp;			
		index=root;
		root=(index-1)/2;
	}	
}
void dec_key_binomial(int x,int newdist,vector<binNode*> &pos_bin)
{
	binNode* node=pos_bin[x-1];
	node->dis=newdist;	
	binNode* parent=node->parent;
	while(parent!=NULL && parent->dis>node->dis)
	{
		//swap
			int temp=node->dis;
			node->dis=parent->dis;
			parent->dis=temp;
			temp=node->node;
			node->node=parent->node;
			parent->node=temp;
			pos_bin[(parent->node)-1]=parent;
			pos_bin[(node->node)-1]=node;
		parent=parent->parent;
		node=parent;
	}
}
void dec_key_fibonacci(int x,int newdist,vector<fibNode*> &pos_fib,list<fibNode*> &fibonacci)
{		
	fibNode* node=pos_fib[x-1];
	node->dis=newdist;				//newdist is always less than old one			
	if(minfib==-1)
		minfib=x;
	else if(newdist<(pos_fib[minfib-1])->dis)		//update min if necessary
	{
		minfib=x;
	}
	if(node->parent==NULL)
		return;
	if(newdist>=node->parent->dis)			//no need to break, since heap property is not violated
		return;
	fibNode* parent;	
	do
	{
		parent=node->parent;
		if(parent->leftchild==node)
			parent->leftchild=node->right;
		if(node->left!=NULL)
		{
			node->left->right=node->right;			
		}
		if(node->right!=NULL)
		{
			node->right->left=node->left;
		}
		parent->degree--;	
		if(parent->parent!=NULL)		//if parent is not root node, mark it	
			parent->mark++;
		node->parent=NULL;	node->left=NULL;node->right=NULL;
		list<fibNode*> :: iterator itr=fibonacci.begin();
		fibNode* start=*itr;
		start->left=node;
		node->right=start;
		node->mark=0;
		fibonacci.push_front(node);
		node=parent;
	}while(node->mark==2);	
}
pair<int,int> extractMin_binary(vector<pair<int,int> > &binHeap,int positions[])
{
	pair<int,int> min;
	min=binHeap[0];	
	binHeap[0].first=INF;
	int left=1,right=2;
	int minchild=INF,minIndex=-1;
	int size=binHeap.size();
	if(left<size && binHeap[left].first<INF)
	{
		minchild=binHeap[left].first;
		minIndex=left;
	}
	if(right<size && binHeap[right].first<minchild)
	{
		minchild=binHeap[right].first;
		minIndex=right;
	}
	int root=0;		//stores index of root
	
		while(minIndex!=-1)			//heapify downwards
		{			
			//swap
				positions[binHeap[root].second-1]=minIndex;
				positions[binHeap[minIndex].second-1]=root;
				pair<int,int> temp=binHeap[root];
				binHeap[root]=binHeap[minIndex];
				binHeap[minIndex]=temp;				
			root=minIndex;
			left=2*root+1;  right=2*root+2;
			minIndex=-1;	minchild=binHeap[root].first;
			if(left<size && binHeap[left].first<minchild)
			{
				minchild=binHeap[left].first;
				minIndex=left;
			}
			if(right<size && binHeap[right].first<minchild)
			{
				minchild=binHeap[right].first;
				minIndex=right;
			}
		}		
	return min;
}
pair<int,int> extractMin_binomial(vector<binNode*> &pos_bin,list<binNode*> &binomial)
{	
	int mindist=INF,minNode=-1;
	list<binNode*> :: iterator itr;
	for(itr=binomial.begin();itr!=binomial.end();itr++)
	{
		if(minNode==-1)
		{
			minNode=(*itr)->node;
			mindist=(*itr)->dis;
		}
		else if((*itr)->dis<mindist)
		{
			minNode=(*itr)->node;
			mindist=(*itr)->dis;
		}			
	}		
	pair<int,int> pi=mp(mindist,minNode);		//the return value
	list<binNode*> l1;
	binNode* minbin=pos_bin[minNode-1];	
	pos_bin[minNode-1]=NULL;
	binNode* temp=minbin->leftchild;
	while(temp!=NULL)
	{
		l1.push_front(temp);			//using push_front so that list is formed in increasing order of degree
		temp->parent=NULL;			//deleting min Node as parent
		temp=temp->rightsib;
	}	
	binomial=union_binomial(l1,binomial,minNode);			//sending minNode since that needs to be excluded during union
	delete minbin;				//freeing up memory
	return pi;
}
pair<int,int> extractMin_fibonacci(vector<fibNode*> &pos_fib, list<fibNode*> &fibonacci)
{
	pair<int,int> pi;
	int mindist=(pos_fib[minfib-1]->dis);
	fibNode* minNode;
	list<fibNode*> ::iterator it3;
	for(it3=fibonacci.begin();it3!=fibonacci.end();it3++)		//we search root list to find element to be deleted. This is done because the minfib Node might not necessarily be the root(if it has the same distance as the root)
	{
		if((*it3)->dis==mindist)
		{
			minNode=(*it3);
			minfib=(*it3)->node;
			break;
		}
	}	
	pos_fib[minfib-1]=NULL;					
	pi=mp(minNode->dis,minfib);	
	minfib=-1;			//next min will be determined later
	if(minNode->right!=NULL)
		minNode->right->left=minNode->left;
	if(minNode->left!=NULL)
		minNode->left->right=minNode->right;
	fibNode* child=minNode->leftchild;
	while(child!=NULL)
	{
		if(minfib==-1)
			minfib=child->node;
		else if(child->dis<(pos_fib[minfib-1])->dis)			
		{
			minfib=child->node;
		}
		fibNode* next=child->right;
		child->parent=NULL; child->right=NULL; child->left=NULL;
		if(fibonacci.size()!=0)
		{	
			if(fibonacci.front()!=minNode)
			{
				 child->right=fibonacci.front();
				(fibonacci.front())->left=child;
			}				
			else			//first element of the heap is the element to be deleted
			{
				child->right=minNode->right;
				if(minNode->right!=NULL)
					minNode->right->left=child;
			}			
		}
		fibonacci.push_front(child);
		child=next;
	}	
	
	list<fibNode*> :: iterator itr;
	vector<fibNode*> degree(64,NULL);			//degree 64 is enough to handle large test cases
	for(itr=fibonacci.begin();itr!=fibonacci.end();itr++)
	{
		if((*itr)==minNode)				//this node has to be deleted from the heap
			continue;
		if(minfib==-1)									//updating min node of fibonacci
			minfib=(*itr)->node;
		else if((*itr)->dis<(pos_fib[minfib-1])->dis)
			minfib=(*itr)->node;

		int d=(*itr)->degree;
		if(degree[d]==NULL)			//same degree heap isn't already there
		{	
			//making the sibling pointers null
			if((*itr)->left!=NULL)
				(*itr)->left->right=(*itr)->right;
			if((*itr)->right!=NULL)
				(*itr)->right->left=(*itr)->left;	
			(*itr)->left=NULL;
			(*itr)->right=NULL;

			degree[d]=*itr;
		}
		else					//same degree heap already exists. perform merge
		{
			repeat:   			//added for creating loop
			fibNode* temp2=degree[d];
			degree[d]=NULL;				//since after merging, degree would have increased
			if(temp2->dis<(*itr)->dis)		//make temp2 root
			{		
				//making the sibling pointers null	
				if((*itr)->left!=NULL)
					(*itr)->left->right=(*itr)->right;
				if((*itr)->right!=NULL)
					(*itr)->right->left=(*itr)->left;	
				(*itr)->right=temp2->leftchild;
				if(temp2->leftchild!=NULL)
					temp2->leftchild->left=(*itr);

				temp2->leftchild=*itr;
				temp2->degree++;
				(*itr)->parent=temp2;				//merge finished

				d=temp2->degree;
				if(degree[d]==NULL)
				{
					degree[d]=temp2;
				}
				else
				{
					(*itr)=temp2;			//now perform the same steps taking new formed node as itr					
					goto repeat;
				}				
			}
			else					//make itr root
			{
				//making the sibling pointers null
				if(temp2->right!=NULL)
					temp2->right->left=temp2->left;
				if(temp2->left!=NULL)
					temp2->left->right=temp2->right;
				temp2->right=(*itr)->leftchild;
				if((*itr)->leftchild!=NULL)
					(*itr)->leftchild->left=temp2;				
				(*itr)->leftchild=temp2;

				(*itr)->degree++;
				temp2->parent=(*itr);				//merge finished

				d=(*itr)->degree;
				if(degree[d]==NULL)
				{
					//making the sibling pointers null
					if((*itr)->left!=NULL)
						(*itr)->left->right=(*itr)->right;
					if((*itr)->right!=NULL)
						(*itr)->right->left=(*itr)->left;	
					(*itr)->left=NULL;
					(*itr)->right=NULL;	
					degree[d]=(*itr);
				}
				else
				{	
					if((*itr)->left!=NULL)
						(*itr)->left->right=(*itr)->right;
					if((*itr)->right!=NULL)
						(*itr)->right->left=(*itr)->left;	
					(*itr)->left=NULL;
					(*itr)->right=NULL;	

					goto repeat;
				}				
			}			
		}
		
	}		
	minNode->left=NULL; minNode->right=NULL; minNode->leftchild=NULL;	minNode->parent=NULL;
	delete minNode;				//freeing up memory
	fibonacci.clear();
	for(int i=0;i<degree.size();i++)
	{
		if(degree[i]!=NULL)
		{
			if(fibonacci.size()!=0)
			{
				degree[i]->left=fibonacci.back();
				(fibonacci.back())->right=degree[i];
			}
			fibonacci.pb(degree[i]);
		}
	}
	return pi;
}
fibNode* newNode_fib(int node1,int dist)
{
	fibNode *temp=new fibNode();
	temp->dis=dist;
	temp->node=node1;
	temp->right=NULL;
	temp->left=NULL;
	temp->leftchild=NULL;
	temp->mark=0;
	temp->degree=0;
	return temp;
}
binNode* newNode_bin(int node1,int dist)
{
	binNode* temp=new binNode();
	temp->node=node1;
	temp->dis=dist;
	temp->degree=0;
	temp->parent=NULL;
	temp->leftchild=NULL;
	temp->rightsib=NULL;
	return temp;
}
list<binNode*> union_binomial(list<binNode*> l1,list<binNode*> l2,int y)			//y node needs to be excluded during union
{
	if(l1.empty())			//base cases
	{
		list<binNode*> :: iterator itr;
		list<binNode*> l3;
		for(itr=l2.begin();itr!=l2.end();itr++)
		{
			if((*itr)->node!=y)
				l3.pb((*itr));
		}
		return l3;
	}		
	if(l2.empty())
		return l1;
	list<binNode*> :: iterator it1,it2;
	it1=l1.begin(); it2=l2.begin();
	list<binNode*> l3;				//final list to be returned
	while(it1!=l1.end() && it2!=l2.end())
	{
		if((*it2)->node==y)
		{
			it2++;
			continue;
		}
		if((*it1)->degree<(*it2)->degree)
		{
			if(l3.size()!=0)
			{
				(l3.back())->rightsib=*it1;
			}
			l3.pb(*it1);
			it1++;
		}
		else if((*it1)->degree>(*it2)->degree)
		{
			if(l3.size()!=0)
			{
				(l3.back())->rightsib=*it2;
			}
			l3.pb(*it2);
			it2++;
		}
		else 		//degree is same
		{
			list<binNode*> :: iterator temp1,temp2;
			temp1=it1; temp2=it2;
			if((*it1)->dis<=(*it2)->dis)
			{
				(*it2)->rightsib=(*it1)->leftchild;
				(*it1)->leftchild=(*it2);
				while((*it2)!=NULL)
				{
					(*it2)->parent=(*it1);
					(*it2)=(*it2)->rightsib;
				}				
				(*it1)->degree++;	
				if(l3.size()!=0)
				{
					(l3.back())->rightsib=*it1;
				}
				l3.pb(*it1);			
			}
			else 
			{
				(*it1)->rightsib=(*it2)->leftchild;
				(*it2)->leftchild=(*it1);
				while((*it1)!=NULL)
				{
					(*it1)->parent=(*it2);
					(*it1)=(*it1)->rightsib;
				}						
				(*it2)->degree++;	
				if(l3.size()!=0)
				{
					(l3.back())->rightsib=*it2;
				}
				l3.pb(*it2);		
			}
			temp1++;temp2++;
			it1=temp1; it2=temp2;
		}
	}
	while(it1!=l1.end())		//remaining binomial trees
	{
		if(l3.size()!=0)		
			(l3.back())->rightsib=*it1;
		l3.pb((*it1));
		it1++;
	}
	while(it2!=l2.end())		//only one of the two loops will be executed
	{
		if((*it2)->node!=y)
		{
			if(l3.size()!=0)			
				(l3.back())->rightsib=*it2;			
			l3.pb((*it2));
		}			
		it2++;
	}
	return l3;
}
