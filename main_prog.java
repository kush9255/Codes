import java.io.File;
import java.util.*;

class main_prog {
    static int stn_cnt = 0;
    public static void main(String args[]) throws Exception {

        int[][] graph = Read_file("File.txt");
        int[] steiner_vertices = stiener_verify(graph.length);

        System.out.println("The 2-factor approximate tree we have computed is given below (we describe this tree by listing all the neighbors of all the graph.length in the tree):");

        int[][] combined_matrix = shortest(graph);
        int[][] metric_steiner_vertices =find_metric_steiner(combined_matrix);
        int[]path_mat=find_path_mat(combined_matrix);
        HashMap < String, ArrayList < Integer >> path = get_path(metric_steiner_vertices, path_mat);

        int[][] mst = findMst(metric_steiner_vertices, steiner_vertices, stn_cnt, path);

        steiner_mst(mst, path, graph);

    }

    public static int[][] Read_file(String filepath) throws Exception {
        File file = new File(filepath);
        Scanner file_reader = new Scanner(file);
        
        Scanner sc2 = new Scanner(System.in);
        
        int graph_size = 0, i = 0;
        
        String s = file_reader.nextLine();
       
        String[] orig_graph = s.split(" ");
       
        graph_size = orig_graph.length;
        int[][] graph = new int[graph_size][graph_size];
       
        while (i < graph_size) {
            orig_graph = s.split(" ");
            graph_size = orig_graph.length;
            if (orig_graph.length != graph_size || graph_size <= 0) {
                System.err.println("Invalid graph matrix");
                System.exit(2);
            } else {
                for (int j = 0; j < graph_size; j++)
                    graph[i][j] = Integer.parseInt(orig_graph[j]);
                i++;
            }
            if (file_reader.hasNextLine())
                s = file_reader.nextLine();
        }

        System.out.println("The input matrix A the program read from the file is displayed below:");
        for (i = 0; i < graph_size; i++) {
            for (int j = 0; j < graph_size; j++) {
                System.out.print(graph[i][j] + " ");
            }
            System.out.println();
        }
        file_reader.close();
        boolean disconnected =true;
        for (i = 0; i < graph_size; i++) {
            for (int j = 0; j < graph_size; j++) {
                if(graph[i][j] !=0)
                disconnected=false;
            }
        }
            if(disconnected)
            {
                System.out.println("Graph is disconnected");
                System.exit(4);
            }
        return graph;
    }
    public static int[] stiener_verify(int graph_size) {
        Scanner sc2 = new Scanner(System.in);
        int[] steiner_vertices = new int[graph_size];
        System.out.println("List all the Steiner vertices  (type * to quit):");
        String s = sc2.nextLine();

        while (s.compareTo("*") != 0) {
            int vertex = Integer.parseInt(s);
            if (vertex > graph_size || vertex <= 0) {
                System.err.println("Invalid Vertex");
                System.exit(3);
            }
            if (steiner_vertices[vertex - 1] == 1) {
                System.err.println("Duplicate entry try");
            }
            else{
            steiner_vertices[vertex - 1] = 1;
            stn_cnt++;
            }
            s = sc2.nextLine();
        }
        return steiner_vertices;
    }

    public static int[][] shortest(int[][] graph) {
        int src = 0;
        int[][] ans = new int[graph.length + 1][graph.length];
        int[] path = new int[graph.length];

        for (src = 0; src < graph.length; src++) {

            boolean[] shrt_path = new boolean[graph.length];
            int[] distance = new int[graph.length];


            for (int i = 0; i < graph.length; i++) {
                distance[i] = Integer.MAX_VALUE;
            }
            path[src] = -1;
            distance[src] = 0;

            for (int i = 0; i < graph.length; i++) {

                int minKey = Integer.MAX_VALUE;
                int vertex = -1;
                for (int h = 0; h < graph.length; h++) {
                    if (shrt_path[h] == false && distance[h] < minKey) {
                        minKey = distance[h];
                        vertex = h;
                    }
                }


                shrt_path[vertex] = true;

                for (int neighbour = 0; neighbour < graph.length; neighbour++) {
                    if (graph[vertex][neighbour] > 0) {

                        if (graph[vertex][neighbour] != Integer.MAX_VALUE && shrt_path[neighbour] != true) {
                            int newKey = graph[vertex][neighbour] + distance[vertex];
                            if (newKey < distance[neighbour]) {
                                distance[neighbour] = newKey;
                                path[neighbour] = vertex;
                            }
                        }
                    }
                }
            }

            for (int iter = 0; iter < graph.length; iter++) {
                ans[src][iter] = distance[iter];
            }

        }
        for (int iter = 0; iter < graph.length; iter++)
            ans[src][iter] = path[iter];

        return ans;

    }

    public static HashMap < String, ArrayList < Integer >> get_path(int[][] graph, int[] path) {
        HashMap < String, ArrayList < Integer >> ans2 = new HashMap < > ();
        for (int src = 0; src < graph.length; src++)
            for (int iter = 0; iter < graph.length; iter++) {
                String key = "" + src + "" + iter;
                ArrayList < Integer > val = new ArrayList < Integer > ();
                int v = iter;
                while (v >= 0) {
                    val.add(v);
                    v = path[v];
                }
                ans2.put(key, val);
            }
        return ans2;
    }
    public static int[][] find_metric_steiner(int[][]combined_matrix)
    {
        int graph_size=combined_matrix.length-1;
        int[][] metric_steiner_vertices = new int[graph_size][graph_size];
        for (int i = 0; i < graph_size; i++)
        for (int j = 0; j < graph_size; j++)
        metric_steiner_vertices[i][j] = combined_matrix[i][j];
        return metric_steiner_vertices;
    }
    public static int[] find_path_mat(int [][] combined_matrix)
    {
        int graph_size=combined_matrix.length-1;
        int[] path_mat = new int[graph_size];
        int k = combined_matrix.length - 1;
        for (int i = 0; i < graph_size; i++)
        path_mat[i] = combined_matrix[k][i];
        return path_mat;   
    }
    public static int[][] findMst(int graph[][], int[] steiner_vertices, int stn_cnt, HashMap < String, ArrayList < Integer >> path) {
        int required_verices = graph.length - stn_cnt;
        int i = 0, j = 0, k = 0, m = 0;
        int[][] graph2 = new int[required_verices][required_verices];
        while(k<graph2.length){
            if(steiner_vertices[i] == 1){
                i++;
                continue;
            }
            j=0; m=0;
            while(m<graph2.length){
                if(steiner_vertices[j] == 1){
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
        
        int[][] ans = new int[required_verices][required_verices];
        int parent[] = new int[required_verices];
        int key[] = new int[required_verices];
        Boolean mstSet[] = new Boolean[required_verices];

        for (i = 0; i < required_verices; i++) {
            key[i] = Integer.MAX_VALUE;
            mstSet[i] = false;
        }
        key[0] = 0;
        parent[0] = -1;

        for (int count = 0; count < required_verices - 1; count++) {

            int min = Integer.MAX_VALUE, min_index = -1;

            for (int v = 0; v < required_verices; v++)
                if (mstSet[v] == false && key[v] < min) {
                    min = key[v];
                    min_index = v;
                }

            int u = min_index;
            mstSet[u] = true;

            for (int v = 0; v < required_verices; v++)
                if (graph2[u][v] != 0 && mstSet[v] == false && graph2[u][v] < key[v]) {
                    parent[v] = u;
                    key[v] = graph2[u][v];
                }
        }
        for (i = 1; i < required_verices; i++) {
            ans[i][parent[i]] = graph2[i][parent[i]];
            ans[parent[i]][i] = graph2[parent[i]][i];
        }
        i = 0;j = 0;k = 0;m = 0;
        int[][] ans2 = new int[graph.length][graph.length];
        
        i = 0;j = 0;k = 0;m = 0;

        while(i<graph.length){
            j=0; m=0;
            if(steiner_vertices[i] == 1){
                i++;
                continue;
            }
            while(j<graph.length){
                if(steiner_vertices[j] == 1){
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
        return ans2;

    }
    public static void steiner_mst(int[][] mst, HashMap < String, ArrayList < Integer >> path, int[][] graph) {
        int[][] ans_matrix = new int[graph.length][graph.length];

        for (int i = 0; i < mst.length; i++) {
            for (int j = 0; j < mst.length; j++) {
                if (mst[i][j] == 0) continue;
                else if (mst[i][j] == graph[i][j]) ans_matrix[i][j] = graph[i][j];
                else {
                    ans_matrix[i][j] = 0;
                    ArrayList < Integer > b = path.get(i + "" + j);
                    for (int y = b.size() - 1; y > 0; y--) {
                        ans_matrix[b.get(y)][b.get(y - 1)] = graph[b.get(y)][b.get(y - 1)];
                        ans_matrix[b.get(y - 1)][b.get(y)] = graph[b.get(y - 1)][b.get(y)];
                    }
                }
            }
        }
        for (int i = 0; i < mst.length; i++) {
            System.out.print("Neighbors of Vertex " + (i + 1) + ": ");
            for (int j = 0; j < mst.length; j++) {
                if (ans_matrix[i][j] > 0) System.out.print((j + 1) + " ");
            }
            System.out.println();
        }

    }
}