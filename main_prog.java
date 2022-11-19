import java.io.File;
import java.util.*;
class helper{
    public int [][] ans;
    public HashMap<Integer, ArrayList<Integer>> path;

    public helper(int [][] ans, HashMap<Integer, ArrayList<Integer>> path){
        this.ans = ans.clone();
        this.path = new HashMap<>();

        for(Integer i: path.keySet())
            this.path.put(i, path.get(i));
        }
    }

class main_prog{

    
    public static void main(String args[]) throws Exception {
        File file = new File("/home/kushal/Downloads/File.txt");
        Scanner sc = new Scanner(file);
        Scanner sc2 = new Scanner(System.in);
        int graph_size = 0, i=0;
        String s = sc.nextLine();
        String[] orig_graph = s.split(" ");
         graph_size = orig_graph.length;
        int[][]graph=new int[graph_size][graph_size];
        while (sc.hasNextLine()){
            orig_graph = s.split(" ");
         graph_size = orig_graph.length;
            if(orig_graph.length != graph_size || graph_size <=0) {
                System.err.println("Invalid graph matrix");
                System.exit(2);
            }
            else{
                for(int j=0;j<graph_size;j++)
                 graph[i][j] = Integer.parseInt(orig_graph[j]);
                i++;
            }
            s=sc.nextLine();
        }
        
        System.out.println("The input matrix A the program read from the file is displayed below:");
        for(i=0;i<graph_size;i++){
            for(int j=0;j<graph_size;j++){
                    System.out.print(graph[i][j]+" ");
                }
                System.out.println();
        }
        sc.close();
        int [] steiner = new int[graph_size];
        for(int k = 0 ;k<graph_size;k++) steiner[k] =0;
        int ste_ver = 0;
        System.out.println("List all the Steiner graph.length (type * to quit):");
         s = sc2.nextLine();

        while(s.compareTo("*")!=0 ){
            int vertex = Integer.parseInt(s);
            if(vertex > graph_size || vertex <=0){
                System.err.println("Invalid Vertex");
                System.exit(3);
            }
            if(steiner[vertex - 1] == 1){ 
                System.err.println("Steiner vertice already exists!");
                s = sc2.nextLine();                                       
            }
            steiner[vertex-1] = 1;
            ste_ver++;
            s = sc2.nextLine();
        }
        System.out.println("The 2-factor approximate tree we have computed is given below (we describe this tree by listing all the neighbors of all the graph.length in the tree):");
        
        int [][] metric_steiner = new int[graph_size][graph_size];
        helper info = shortest(graph);

        metric_steiner = info.ans.clone();

        for(i=0;i<graph_size;i++){
            for(int j=0;j<graph_size;j++){
                    System.out.print(metric_steiner[i][j]+" ");
                }
                System.out.println();
        }
        
  

        
        if(graph_size -ste_ver <= 1){
            System.err.println("No tree can be computed when there are 0 or 1 required graph.length!");
            System.exit(3);
        }
        int [][] mst_required = primMST(metric_steiner,steiner, ste_ver);

        int [][] ans_matrix = new int[graph_size][graph_size];

        for(i=0;i<mst_required.length;i++){
                 for(int j=0;j<mst_required.length;j++){
                        if(mst_required[i][j] == 0) continue;
                        else if(mst_required[i][j] == graph[i][j]) ans_matrix[i][j] = graph[i][j];
                        else{
                            ans_matrix[i][j] = 0;
                            ArrayList<Integer> b = info.path.get(hash(i,j));
                            for(int y = b.size()-1; y>0; y--){
                                ans_matrix[b.get(y)][b.get(y-1)] = graph[b.get(y)][b.get(y-1)];
                                ans_matrix[b.get(y-1)][b.get(y)] = graph[b.get(y-1)][b.get(y)];
                            }
                        }
                     }
        }

        for(i=0;i<mst_required.length;i++){
            System.out.print("Neighbors of Vertex "+(i+1)+": ");
            for(int j=0;j<mst_required.length;j++){
                    if(ans_matrix[i][j] > 0) System.out.print((j+1)+" ");
                }
                System.out.println();
        }
    }



    public static helper shortest(int [][] graph){

        int [][] ans = new int [graph.length][graph.length];
        HashMap<Integer, ArrayList<Integer>> ans2 = new HashMap<>();

        for(int src =0 ;src<graph.length;src++){
    
            boolean[] spt = new boolean[graph.length];
            int [] distance = new int[graph.length];
            int [] path = new int[graph.length];

            for (int i = 0; i <graph.length ; i++) {
                distance[i] = Integer.MAX_VALUE;
            }
            path[src] = -1;
            distance[src] = 0;

            for (int i = 0; i <graph.length ; i++) {

                int minKey = Integer.MAX_VALUE;
                int vertex = -1;
                for (int h = 0; h <graph.length ; h++) {
                    if(spt[h]== false && minKey>distance[h]){
                        minKey = distance[h];
                        vertex = h;
                    }
                }

           
                spt[vertex] = true;
           
                for (int vertex_V = 0; vertex_V <graph.length ; vertex_V++) {
                    if(graph[vertex][vertex_V]>0){
           
                        if(spt[vertex_V]==false && graph[vertex][vertex_V]!=Integer.MAX_VALUE){
          
                            int newKey = graph[vertex][vertex_V] + distance[vertex];
                            if(newKey<distance[vertex_V]){
                                distance[vertex_V] = newKey;
                                path[vertex_V] = vertex;
                            }
                        }
                    }
                }
            }

            for(int p=0; p<graph.length; p++) ans[src][p] = distance[p];
            for(int p=0; p<graph.length; p++) ans2.put(hash(src,p),get_path(path, p));

        }
        return new helper(ans, ans2);

    }
    public static ArrayList<Integer> get_path(int [] path, int k){
            ArrayList<Integer> ans = new ArrayList<Integer>();
            while(k>=0){
                ans.add(k);
                k = path[k];
            }
            return ans;
    }

    public static int hash(int i, int j){
        return (i+13) * (j+13) *(i+j+26);
    }
    
    public static int[][] primMST(int graph[][], int [] steiner, int ste_ver)
    {

        

        int V = graph.length - ste_ver;
        int i = 0,j = 0, k =0 , m=0;
        int [][] graph2 = new int[V][V];
        while(k<graph2.length){
            if(steiner[i] == 1){
                i++;
                continue;
            }
            j=0; m=0;
            while(m<graph2.length){
                if(steiner[j] == 1){
                    j++;
                    continue;
                }
                graph2[k][m] = graph[i][j];
                m++;
                j++;
            }
            k++;
            i++;
        }
        for(i=0;i<V;i++){
            for(j=0;j<V;j++){
                    System.out.print(graph2[i][j]+" ");
                }
                System.out.println();
        }
        int [][] ans = new int[V][V];
        for(i=0;i<V;i++){
            for(j=0;j<V;j++){
                    ans[i][j] = 0;
                }
        }
        int parent[] = new int[V];
 
       
        int key[] = new int[V];
       
        Boolean mstSet[] = new Boolean[V];
 
        for (i = 0; i < V; i++) {
            key[i] = Integer.MAX_VALUE;
            mstSet[i] = false;
        }
        key[0] = 0;
        parent[0] = -1;
 
        for (int count = 0; count < V - 1; count++) {

            int min = Integer.MAX_VALUE, min_index = -1;
 
            for (int v = 0; v < V; v++)
                if (mstSet[v] == false && key[v] < min) {
                    min = key[v];
                    min_index = v;
                }

            int u = min_index;
            mstSet[u] = true;
 
            for (int v = 0; v < V; v++)
                if (graph2[u][v] != 0 && mstSet[v] == false && graph2[u][v] < key[v]) {
                    parent[v] = u;
                    key[v] = graph2[u][v];
                }
        }
        for(i=1;i<V;i++){
            ans[i][parent[i]] = graph2[i][parent[i]];
            ans[parent[i]][i] = graph2[parent[i]][i];
        }
        for(i=0;i<V;i++){
            for(j=0;j<V;j++){
                    System.out.print(ans[i][j]+" ");
                }
                System.out.println();
        }
        i = 0;j = 0; k =0; m=0;
        int [][] ans2 = new int[graph.length][graph.length];
        for(i=0;i<graph.length;i++)
            for(j=0;j<graph.length;j++) ans2[i][j] = 0;
        i = 0;j = 0; k =0; m=0;

        while(i<graph.length){
            j=0; m=0;
            if(steiner[i] == 1){
                i++;
                continue;
            }
            while(j<graph.length){
                if(steiner[j] == 1){
                    j++;
                    continue;

                }
                else{
                    ans2[i][j] = ans[k][m];
                    m++;
                    j++;
                        
                }
            }
            k++;
            i++;
        }
        for(i=0;i<graph.length;i++){
            for(j=0;j<graph.length;j++){
                    System.out.print(ans2[i][j]+" ");
                }
                System.out.println();
        }
            

        return ans2;

    }
}
