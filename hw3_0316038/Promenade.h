#include<iostream>
#define MAX 100000
#define MIN -100000
using namespace std;

class Person{
public:
	Person();
	Person(int i, int e);
	~Person();
	int set_energy(int e){energy=e;}
private:
	Person *parent, *child, *sibling; // the pointers to his parent, child, sibling
	int id; // record Person's id
	int energy; // record Person's energy
	int degree;// record how many levels of this Person's children
	friend class Promenade;//you could access all valuables in class Promenade
};

//ctor
Person::Person(){
	parent = child = sibling = NULL;
	id = 0;
	energy = 0;
	degree = 0;
}

//ctor
Person::Person(int i, int e){// define a new Person with id = i and energy = e
	parent = child = sibling = NULL;
	id = i;
	energy = e;
	degree = 0;
}

//dtor
Person::~Person(){

}

class Promenade{

public:
	Promenade();
	~Promenade();
	void one_person_joined(int id, int energy);
	void one_group_joined(Promenade *g);
	void absorb_energy(int id, int energy);
	int  calculate_people_below(int energy);
	int  get_weakest_person();
	int  size();
	void mergeheap(Promenade *g);
	int rune(Person *p,int energy);
	Person* search(Person *p,int id);
private:
	Person *head;
	int num;//caculate how many people in the promenade
};

//ctor
Promenade::Promenade(){
	head = NULL;
	num = 0;
}

//dtor
Promenade::~Promenade(){

}
//insert 
void Promenade::one_person_joined(int id, int energy){
	//TODO: fill in the code to simulate a person with his id and energy to join the Promenade
	Promenade bh;
	bh.head=new Person();
	bh.head=new Person(id,energy);
	one_group_joined(&bh);
	num++;
}
//merge
void Promenade::one_group_joined(Promenade *g){
	//TODO: fill in the code to simulate one group to join the full promenade
	mergeheap(g);

	Person *prev=NULL;
	Person *x=head;
	Person *next=x->sibling;
	while(next){
		if(x->degree!=next->degree || next->sibling && next->sibling->degree==x->degree){
			prev=x;
			x=next;
		}
		else if(x->energy<=next->energy){//往前合併 
			x->sibling=next->sibling;
			next->parent=x;
			next->sibling=x->child;
			x->child=next;
			x->degree++; 
		}
		else{//往後合併 
			if(!prev){
				head=next; //更新head 指標
			}
			else{
				prev->sibling=next;
			}
			x->parent=next;
			x->sibling=next->child;
			next->child=x;
			next->degree++;
			x=next;
		}
		next=next->sibling;
	}
}

void Promenade::mergeheap(Promenade *g){
	num+=g->num;
	Person *head2=g->head;
	Person *head1=head; 
	Person *newhead,*newcurr;
	if(!head1){    //如果本身是空的，就不需要合併，直接指向對方即可
		head=head2;
		return;
	}
	else if(!head2){ //對方是空的，也不需要合併
		return;
	}
	//先行尋找誰的開頭比較小，當做新串列的頭
	if(head1->degree>head2->degree){
		newhead=newcurr=head2;
		head2=head2->sibling;
	}
	else{
		newhead=newcurr=head1;
		head1=head1->sibling;
	}
	while(head1&&head2){
		if(head1->degree<head2->degree){
			newcurr->sibling=head1;
			newcurr=head1;
			head1=head1->sibling;
		}
		else{
			newcurr->sibling=head2;
			newcurr=head2;
			head2=head2->sibling;
		}
	}
	while(head1){
		newcurr->sibling=head1;
		newcurr=head1;
		head1=head1->sibling;
	}
	while(head2){
		newcurr->sibling=head2;
		newcurr=head2;
		head2=head2->sibling;
	}
	head=newhead;
}

void Promenade::absorb_energy(int id, int energy){
	//TODO: Elizabeth absord the energy of the person with this id, this Person's energy will decrease by this number.
	//		If his energy becomes lower than or equal to zero, he will die and should be thrown out automatically
	Person *h=head;
	Person *p;
	Person *y;
	Person *z; 
	p=search(h,id);
	while(p==NULL){
		h=h->sibling;
		p=search(h,id);
	}
	if((p->energy)-energy<=0){
		num--;
		p->energy=(p->energy)-energy;//0
		int temp,temp2;
		y=p;//0
		z=p->parent;//5
		while(z!=NULL&&y->energy<z->energy){
			temp=y->energy;
			temp2=y->id;
			y->energy=z->energy;
			y->id=z->id;
			z->energy=temp;
			z->id=temp2;
			y=z;
			z=z->parent;
		}

		if(y==head){
			head=y->sibling;  //renew head pointer
		} 
		else{
			Person *h=head;
			Person *pre;
			while(h->sibling){
				if(h->sibling->energy!=y->energy){
					pre=h;
					h=h->sibling;
				}
				else{
					pre=h;
					break;
				}
			}
			pre->sibling=y->sibling;
		}
		Person *newhead=NULL;  //reverse
		Person *curr=y->child;//5
		while(curr!=NULL){
			Person *next=curr->sibling;
			curr->sibling=newhead;
			curr->parent=NULL;
			newhead=curr;
			curr=next;
		}
		Promenade bh;
		bh.head=newhead;
		one_group_joined(&bh);
	} 
	
	else{
		int temp,temp2;
		p->energy=(p->energy)-energy;
		y=p;
		z=p->parent;
		while(z!=NULL&&y->energy<z->energy){//bubble up
			temp=y->energy;
			temp2=y->id;
			y->energy=z->energy;
			y->id=z->id;
			z->energy=temp;
			z->id=temp2;
			y=z;
			z=z->parent;
		}
	}

}

int Promenade::calculate_people_below(int energy){
	//TODO: You should calculate how many people's energy are below the number (include the number) and return it
	if(head!=NULL){
		return rune(head,energy);
	}
	return 0;
}

int Promenade::get_weakest_person(){
	//TODO: return the id of the one with the least energy left. If there are more than one solution, just return one of them
	Person *x=head;
	//cout<<x->energy<<endl;
	Person *small=x;
	int min=100000;
	while(x!=NULL){
		if(x->energy<min&&x->energy>0){
			min=x->energy;
			small=x;
		}
		x=x->sibling;
	}
	return small->id;
}

int Promenade::size(){
	
	return num;
}


int Promenade::rune(Person *p,int energy){
	if(!p)	return 0;
	else if(p->energy<=energy){
		return 1+rune(p->child,energy)+rune(p->sibling,energy);
	}
	else return rune(p->sibling,energy);
}


Person* Promenade::search(Person *h,int id){
	Person* x=h;
	Person* p=NULL;
	if(x->id==id){
		p=x;
		return p;
	}
	if(x->child!=NULL&&p==NULL){
		p=search(x->child,id);
	}
	if(x->sibling!=NULL&&p==NULL){
		p=search(x->sibling,id);
	}
	return p;
}