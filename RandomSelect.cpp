#include <iostream>
#include <ctime>
#include<cstdlib>
using namespace std;

int comparisons;


int rand_partition(int arr[],int low, int high){
	int pivotIndex= low+rand()%(high-low+1);
	swap(arr[pivotIndex],arr[high]);
	int pivot = arr[high];
	int i = low - 1;
	for(int j=low;j <= high -1;j++){
		comparisons++;
		if(arr[j] <= pivot){
			i++;
			swap(arr[i],arr[j]);
		}
	}
	swap(arr[i+1],arr[high]);
	return (i+1);
}

int rand_select(int A[],int p, int r, int i){
	if(p==r){
		return A[p];
	}
	int q,k;
	q=rand_partition(A,p,r);
	k=q-p+1;
	if(i==k){
		return A[q];
	}
	else if(i<k){
		return rand_select(A,p,q-1,i);
	}
	else{
		return rand_select(A,q+1,r,i-k);
	}
}


int main(){
	srand(time(0));
	
	int n,*a;
	cout<<"-> Enter the size of array : ";
	cin>>n;
	a=new int[n];
	
	cout<<"-> Enter array elements : ";
	for (int i=0;i<n;i++){
		cin>>a[i];
	}
	
	int rank;
	cout<<"\nEnter the index to be found: ";
	cin>>rank;
	
	if(rank>n){
		cout<<"\nInvalid Index!!!!";
		exit(0);
	}
	
	cout<<"\n\n-> Initial array : ";
	for (int i=0;i<n;i++){
		cout<<a[i]<<"\t";
	}
	cout<<endl;
	int ans;
	ans=rand_select(a,0,n-1,rank);
	
	cout<<"\n\n-> Array after calling Random Select : ";
	for (int i=0;i<n;i++){
		cout<<a[i]<<"\t";
	}
	
	cout<<"\n\n-> Element at requested index : "<<ans;
	
	cout<<"\n\n No. of comparisons: "<<comparisons;
	
	
	return 0;
}
