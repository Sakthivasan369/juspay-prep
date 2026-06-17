#include<bits/stdc++.h>
using namespace std;

string solve (string s, int k){
     set<char> uniq(s.begin(),s.end());
     vector<char> chars(uniq.begin(),uniq.end());
     string ans=s;
     if(k>=n){
          char smallest=chars[0];
          while(k-n>=0){
              ans+=smallest;
          }
     }
     else{
          ans=s.substr(0,k);
          for(int i=k-1;i>=0;i--){
          auto it=upper_bound(chars.begin(),chars.end(),ans[i])
           if(it!=chars.end()){
                char a=*it;
                for(int  j=i;j<k;j++){
                     ans[j]=a;
                }
                return ans;
           }
        
        }
        
     }
return ans;

}
int main(){
     int k;string s;
     cin>>k>>s;
     cout<<solve(k,s)<<" solved";
     return 0;
}