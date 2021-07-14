using System.Collections;
using System.Collections.Generic;

public class MapAreaNode
{
    public MapAreaNode(){
        next = 0;
        start = 0;
        end = 0;
    }
    
    public int next;
    public int start;
    public int end;

    public void SetMapAreaNode(int next,int start,int end){
        this.next = next;
        this.start = start;
        this.end = end;
    }
    public void AddNext(){
        // 最終ノードの以外の場合、ノードを加算する
        if(!CheckNodeReset())
        this.next++;
    }
    private bool CheckNodeReset(){
        // 最終ノードの場合、最初のノードに変更
        if(this.next >= this.end){
            this.next = this.start;
            return true;
        }
        return false;
    }
    public int GetNext(){
        return this.next;
    }

    
}
