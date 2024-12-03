#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

int comparisons;
void swap(int& a, int& b){
	int c = a;
	a = b;
	b = c;
}

int partition(int arr[], int low, int high){
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

void randomizedQuickSort(int arr[], int low, int high){
	if(low<high){
		int pi=partition(arr,low, high);
		randomizedQuickSort(arr,low,pi-1);
		randomizedQuickSort(arr,pi+1,high);
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
	
	
	cout<<"\n\n-> Initial array : ";
	for (int i=0;i<n;i++){
		cout<<a[i]<<"\t";
	}
	cout<<endl;
	randomizedQuickSort(a,0,n-1);
	
	cout<<"-> Sorted array : ";
	for (int i=0;i<n;i++){
		cout<<a[i]<<"\t";
	}
	
	return 0;
}
