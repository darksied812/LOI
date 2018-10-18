#include<iostream>
#include<vector>
#include <fstream>
#include <string>
#include <math.h>
#include<algorithm>
using namespace std;

void print(vector< vector<int> > ans)
{
     for(int i=0;i<ans.size();i++)
    {
        cout<<"BIN # "<<i+1<<" contains Element Number : ";
        for(int j=0;j<ans[i].size();j++)
            cout<<ans[i][j]+1<<" ";
        cout<<endl;
    }
    cout<<endl;
}

void generateCombinations(vector< vector<int> > &v,vector<int> &row,vector<int> &A,int B,int i,vector<int> &vis)
{
    if(B<0)
        return;

    if(B==0 || A.size()==i)
    {
        v.push_back(row);
        return;
    }

    if(vis[i])
    {
        row.push_back(i);
        generateCombinations(v,row,A,B-A[i],i+1,vis);
        row.pop_back();
        generateCombinations(v,row,A,B,i+1,vis);
    }
    else
    {
        generateCombinations(v,row,A,B,i+1,vis);
    }
}

vector< vector<int> > mbs(vector<int> weights,int c)
{
    vector<int> vis(weights.size(),-1),bin;
    vector< vector<int> > bins,temp;

    for(int i=0;i<weights.size();i++)
    {
        if(vis[i]==-1)
        {
            temp.clear();
            bin.clear();
            vis[i]=0;
            bin.push_back(i);
            generateCombinations(temp,bin,weights,c-weights[i],0,vis);
            
            int miin=c,index=0;
        
            for(int i=0;i<temp.size();i++)
            {
                int count=0;
                
                for(int j=0;j<temp[i].size();j++)
                    count+=weights[temp[i][j]];
                
                if((c-count)<miin)
                    index=i,miin=c-count;
            }
        
            bins.push_back(temp[index]);
            for(int i=0;i<temp[index].size();i++)
                vis[temp[index][i]]=0;
        }
    }

    return bins;
}

vector< vector<int> > bestFit(vector<int> weight,int c)
{
    int n=weight.size(),res;
    vector<int> bin(n),temp;
    vector<vector<int> > ans(n,vector<int>());

    for (int i=0; i<n; i++)
    {
        int min = c+1, bi = 0;

        for (int j=0; j<res; j++)
            if (bin[j] >= weight[i] && bin[j] - weight[i] < min)
                bi = j,min = bin[j] - weight[i];
 
        if (min==c+1)
            bin[res] = c - weight[i],ans[res++].push_back(i);

        else 
            bin[bi] -= weight[i],ans[bi].push_back(i);
    }
    ans.erase(ans.begin()+res,ans.begin()+n);
    return ans;
}

int improvedLB(vector<int> weight,int c)
{
    int k=c/2,ans=0,temp;
    
    if(weight[weight.size()-1]>c/2)
        return weight.size();

    for(int i=0;i<weight.size();i++)
    {
        int n1=0,n2=0,s2=0,s3=0;
        
        if(weight[i]>k)
            continue;
        else
        {
            k=weight[i];
            for(int j=0;j<weight.size();j++)
            {
                if(weight[j]>c-k)
                    n1++;
                else if(weight[j]>(c/2) && weight[j]<=(c-k))
                    n2++,s2+=weight[j];
                else if(weight[j]>=k && weight[j]<=(c/2))
                    s3+=weight[j];
            }
            
            temp=n1+n2+max(0,(int)ceil(((s3-(n2*c)+s2)*1.0)/(c*1.0)));
            
            if(temp>=ans)
                ans=temp;
            else
                break;
        }
    }
    return ans;
}

vector< vector<int> > pmbs(vector<int> weight, vector< vector<int> > prevsol, int l2, int c)
{
    vector<int> vis(weight.size(),-1),bin;
    vector< vector<int> > bins,temp,fans;
    fans=prevsol;

    for(int k=0;k<1000;k++)
    {
        if(prevsol.size()==l2)
            return prevsol;
        
        int ind=0,ans=0;
        
        for(int i=0;i<prevsol.size();i++)
        {
            int count=0;
            
            for(int j=0;j<prevsol[i].size();j++)
                count+=weight[prevsol[i][j]];
            
            if(c-count>ans)
                ind=i,ans=c-count;
        }

        vis[prevsol[ind][0]]=0;
        bin.push_back(prevsol[ind][0]);
        generateCombinations(temp,bin,weight,c-weight[prevsol[ind][0]],0,vis);
        int miin=c,index=0;
        
        for(int i=0;i<temp.size();i++)
        {
            int count=0;
                    
            for(int j=0;j<temp[i].size();j++)
                count+=weight[temp[i][j]];
                
            if((c-count)<miin)
                index=i,miin=c-count;
        }
        
        bins.push_back(temp[index]);
            
        for(int i=0;i<temp[index].size();i++)
            vis[temp[index][i]]=0;

        for(int i=0;i<weight.size();i++)
        {
            if(vis[i]==-1)
            {
                temp.clear();
                bin.clear();
                vis[i]=0;
                bin.push_back(i);
                generateCombinations(temp,bin,weight,c-weight[i],0,vis);
                int miin=c,index=0;
        
                for(int i=0;i<temp.size();i++)
                {
                    int count=0;
                    
                    for(int j=0;j<temp[i].size();j++)
                        count+=weight[temp[i][j]];
                
                    if((c-count)<miin)
                        index=i,miin=c-count;
                }
        
                bins.push_back(temp[index]);
            
                for(int i=0;i<temp[index].size();i++)
                    vis[temp[index][i]]=0;
            }
        }

        if(bins.size()<=fans.size())
            fans=bins;
        prevsol=bins;
    }
    return fans;
}

int main()
{
    int choice;
    bool fileInput=true;
    system("clear");

    cout<<"\nPlease Select Mode for Input : \n1:Manual\n2:File\n";
    cin>>choice;
    
    if(choice==1)
        fileInput=false;

    int numberOfBoxes,capacityOfBin,temp;
    vector<int> weight;

    if(fileInput)
    {
        ifstream infile;
        infile.open("input2.txt");
        infile>>numberOfBoxes>>capacityOfBin;

        for(int i=0;i<numberOfBoxes;i++)
            infile>>temp,weight.push_back(temp);

        infile.close();
    }

    else
    {
        cout<<"Enter # of Boxes and Capacity of Bins : \n";
        cin>>numberOfBoxes>>capacityOfBin;
        cout<<"\nEnter size of Boxes : \n";
        for(int i=0;i<numberOfBoxes;i++)
            cin>>temp,weight.push_back(temp);
    }
    
    system("clear");
    sort(weight.begin(),weight.end(),greater<int>());
    
    vector< vector<int> > bins=mbs(weight,capacityOfBin);
    vector< vector<int> > bfd=bestFit(weight,capacityOfBin);
    int l2=improvedLB(weight,capacityOfBin);
    vector< vector<int> > pmb=pmbs(weight,bins,l2,capacityOfBin);

    cout<<"---------------------------------------------------------------\n";
    cout<<"Lower Bound Detected                  : "<<l2<<endl;
    cout<<"---------------------------------------------------------------\n\n";
    cout << "Number of bins required in Best Fit : "<<bfd.size()<<endl<<endl;
    print(bfd);
    cout<<"---------------------------------------------------------------\n\n";
    cout<<"Number of bins required in MBS'       : "<< bins.size()<<endl<<endl;
    print(bins);
    cout<<"---------------------------------------------------------------\n\n";
    cout<<"Number of bins required in Pertubated MBS': "<< pmb.size()<<endl<<endl;
    print(pmb);   
    return 0;
}
