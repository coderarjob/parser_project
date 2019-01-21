/*
* Compile: gcc subset_constrution.c
*/
#include<stdio.h>

typedef struct ec_set{
	int states[10];
	int count;
}Set;


Set dfa_states[9] = {};
int dfa[9][2] = {};
int dfa_count = 0;

Set states[11][3] = {
						{{{1,7},2},{},{}},		// 0
						{{{2,4},2},{},{}},		// 1
						{{},{{3},1},{}},		// 2
						{{{6},1},{{}},{}},		// 3
						{{},{},{{5},1}},		// 4
						{{{6},1},{},{}},		// 5
						{{{1,7},2},{},{}},		// 6
						{{},{{8},1},{}},		// 7
						{{},{},{{9},1}},		// 8
						{{},{},{{10},1}},		// 9
						{{},{},{}}				// 10
					};

void printSet(Set *s);

int set_add(Set *s, int item)
{
	for(int i = 0;i < s->count;i++)
		if (s->states[i] == item) 
			return 0;

	s->states[s->count++] = item;
	return 1;
}

int set_union(Set *a, Set *b)
{
	for(int i = 0; i < b->count; i++)
		set_add(a,b->states[i]);
}

int set_is_equal(Set *a, Set *b)
{
	if (a->count != b->count)
		return 0;

	int found = 1;
	for(int ia=0;
			found == 1 && ia < a->count;
			ia++)
	{
		found = 0;
		for(int ib = 0;
				found == 0 && ib < b->count;
				ib++)
		{
			if (b->states[ib] == a->states[ia])
				found = 1;
		}
	}
	return found;
}

void epsilon_closure(Set *in,Set *s)
{
	int *item = in->states;
	for(int i = 0; i< in->count;i++){
		if (set_add(s, *item)){
			//printf("state: %d, going with: ",*item);
			//printSet(&states[*item][0]);
			epsilon_closure(&states[*item++][0],s);
		}
	}
}


int find_set(Set sets[],int count,Set *item)
{
	for(int i=0; i< count;i++){
		if (set_is_equal(&sets[i],item)){
			return i;
		}
	}
	
	return -1;
}

void subset_construction(Set *s)
{
	//printf("\nsubset construction: ");
	//printSet(s);

	int index = find_set(dfa_states,dfa_count,s);
	if (index == -1){
		dfa_states[dfa_count] = *s;		//add to dfa_states
		index = dfa_count++;
	}

	for(int n = 0;n < 2; n++){
		Set m = {},
			m1 = {};
		for (int i = 0; i < s->count; i++){
			set_union(&m,&states[s->states[i]][n+1]);
		}
		epsilon_closure(&m,&m1);

		/*printf("n = %d\n",n);
		printSet(&m);
		printSet(&m1);*/
		
		int nextstate_index;
		
		if ((nextstate_index = find_set(dfa_states,dfa_count,&m1)) == -1){
			//printf("Not found\n");
			dfa_states[dfa_count] = m1;		//add to dfa_states
			dfa[index][n] = dfa_count++;
			subset_construction(&m1);
		}else{
			//printf("Found\n");
			dfa[index][n] = nextstate_index;
		}
	}
}

void printSet(Set *s)
{
	printf("Count: %d {",s->count);
	for(int i = 0; i < s->count;i++) 
		printf("%d%c",
				s->states[i],
				(i == s->count-1)?'\0':','
			  );
	printf("}\n");
}

int main()
{
	// epsilon closure output states
	Set out = {};

	Set initial_state = {{0},1};
	epsilon_closure(&initial_state,&out);
	subset_construction(&out);
	
	printf("Total state count: %d\n",dfa_count);
	for(int i = 0; i < dfa_count; i++){
		printf("DFA state: %d - ",i);
		printSet(&dfa_states[i]);
	}
	printf("  - a    b\n");
	for(int i = 0; i < dfa_count; i++){
		printf("%d - %d    %d\n",i,dfa[i][0],dfa[i][1]);
	}

	
}
