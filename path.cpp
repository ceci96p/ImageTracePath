#include "path.h"
#include <queue>
#include <stack>
using namespace std;

path::path(const PNG & im, pair<int,int> s, pair<int,int> e)
   :start(s),end(e),image(im){
    BFS();
}

void path::BFS(){
	// initialize working vectors
	vector<vector<bool>> V(image.height(), vector<bool> (image.width(),false));
	vector<vector<pair<int,int>>> P(image.height(), vector<pair<int,int>> (image.width(),end));
 
    // Create a queue for BFS
    queue<pair<int,int>> q;
 
    // Mark the current node as visited and enqueue it
    V[start.second][start.first] = true;
    q.push(start);
 
    while(!q.empty()){
        // Dequeue a vertex from queue 
       std::pair <int,int> s; 
        s = q.front();
        //cout << s << " ";
        q.pop();
 
        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited, 
        // then mark it visited and enqueue it
      vector<pair<int,int>> n;
      n = neighbors(s);

        for (unsigned long i = 0; i < n.size() ; ++i){ 
          if (good(V,s,n[i]) == true){
                V[n[i].second][n[i].first] = true;
                int fi = n[i].first;
                int se = n[i].second;
                std::pair <int,int> adj = make_pair(fi,se); 
                q.push(adj);
                P[n[i].second][n[i].first] = s;///How to update predecessor vector
          }   
        }

     }

	pathPts = assemble(P,start,end);
}

PNG path::render(){
  //copy PNG
  PNG copy = image;
  //copy = PNG(image.width(), image.height());

  /*for(unsigned int y = 0; y< image.height(); y++){
    for(unsigned int x = 0; x< image.width(); x++){
      RGBAPixel *pixel_im = image.getPixel(x,y);
      RGBAPixel *pixel_copy = copy.getPixel(x,y);

      *pixel_copy = *pixel_im;
    }
  }*/
  //change choosen pixels to red
  for(unsigned int i = 0; i< pathPts.size(); i++){
   RGBAPixel *pixel = copy.getPixel(pathPts[i].first, pathPts[i].second);
   (*pixel).r = 255;
   (*pixel).b = 0;
   (*pixel).g = 0;


  }
  //render
  return copy; 
  }
  

vector<pair<int,int>> path::getPath() { return pathPts;}

int path::length() { return pathPts.size();}

bool path::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
 if ((0 <= next.first) && (next.first < (int) image.width()) && (0 <= next.second) && (next.second < (int)image.height())){
  if (v [next.second][next.first] == false){
    if (closeEnough(*image.getPixel(curr.first, curr.second), (*image.getPixel(next.first, next.second)))){
      return true;
    }
  }
}
return false;
}



vector<pair<int,int>> path::neighbors(pair<int,int> curr) {
	vector<pair<int,int>> n;
  std::pair <int,int> above = make_pair(curr.first,curr.second - 1);  
  std::pair <int,int> below = make_pair(curr.first,curr.second + 1); 
  std::pair <int,int> left = make_pair(curr.first - 1, curr.second); 
  std::pair <int,int> right = make_pair(curr.first + 1, curr.second); 

  n.push_back(left);
  n.push_back(below);
  n.push_back(right);
  n.push_back(above);

  return n;

}

vector<pair<int,int>> path::assemble(vector<vector<pair<int,int>>> & p,pair<int,int> s, pair<int,int> e) {
 
    // hint, gold code contains the following line:
	stack<pair<int,int>> S; 
  vector<pair<int,int>> path;

  std::pair <int,int> end = make_pair(e.first,e.second);  
      
  if (p[e.second][e.first] == end){
    std::pair <int,int> pt = make_pair(s.first,s.second); 
    path.push_back(pt);
  }

  else{
        int val_sec = e.second;
        int val_fis = e.first;

        while (!(( p[val_sec][val_fis] == p[s.second][s.first]))){//(!((val_sec == s.second) && (val_fis == s.first))){
        std::pair <int,int> val_pair = make_pair(p[val_sec][val_fis].first, p[val_sec][val_fis].second);
        S.push(val_pair);
        val_sec = val_pair.second;
        val_fis = val_pair.first;
        }

        S.push(p[val_sec][val_fis]);

        while(!S.empty()){
        std::pair <int,int> pt = make_pair(S.top().first,S.top().second);  
        path.push_back(pt);
        S.pop();
        }
}
return path;

}


bool path::closeEnough(RGBAPixel p1, RGBAPixel p2){
   int dist = (p1.r-p2.r)*(p1.r-p2.r) + (p1.g-p2.g)*(p1.g-p2.g) +
               (p1.b-p2.b)*(p1.b-p2.b);

   return (dist <= 80);
}
