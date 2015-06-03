//package test;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @author Mattia Zago <a href="mailto:mattia.zago@studenti.univr.it">mattia.zago@studenti.univr.it"</a>
 * @author Stefano Angeleri <a href="mailto:stefano.angeleri@studenti.univr.it">stefano.angeleri@studenti.univr.it"</a>
 * @author Stefano Maistri <a href="mailto:stefano.maistri@studenti.univr.it">stefano.maistri@studenti.univr.it"</a>
 * @author Umberto Vimercati <a href="mailto:umberto.vimercati@studenti.univr.it">umberto.vimercati@studenti.univr.it"</a>
 */
public class Verificatore {

    public Boolean[] userVC;

    public int OPT = 0;
    public int APX = 0;
    public int vertex = 0;
    public int edges = 0;

    public int userBestSoFar = Integer.MAX_VALUE;

    public Graph graph;

    public class Graph {
        
        private Integer edges = 0;

        public Graph() {
            this.nodes = new LinkedList<>();
            for (int i = 0; i < vertex; i++) {
                this.nodes.add(new Node());
            }
        }

        public List<Node> nodes;

        public void addEdge(int from, int to) {
            if(!this.nodes.get(from).neighbour.contains(to)) {
                this.nodes.get(from).neighbour.add(to);
                this.nodes.get(to).neighbour.add(from);
                edges++;
            }
        }
        
        @Override
        public String toString() {
            String result = this.nodes.size()+";"+this.edges+"\n";
            
            Boolean[] printed = new Boolean[this.nodes.size()];
            Arrays.fill(printed, false);
            
            for(int n=0; n<this.nodes.size(); n++) {
                printed[n] = true;
                for(Integer ne : this.nodes.get(n).neighbour) {
                    if(!printed[ne]) {
                        result+= n+";"+ne+"\n";
                    }
                }
            }
            return result;
        }
    }

    public class Node {

        public List<Integer> neighbour = new LinkedList<>();
        
    }
    
    public static final int EXIT_FOUND_OPTIMAL_SOLUTION = 0;
    public static final int EXIT_MISS_ARGUMENTS = 1;
    public static final int EXIT_INPUT_FILE_NOT_FOUND = 2;
    public static final int EXIT_RESULT_FILE_NOT_FOUND = 3;
    public static final int EXIT_OUTPUT_FILE_NOT_FOUND = 4;

    public static void error(String message, int exitStatus) {
        System.err.println(message);
        System.exit(exitStatus);
    }
    
    public static void main(String args[]) throws IOException {

        if(args.length != 3)
            error("Usage: java Verificatore <I> <R> <O>\n"
                    + "Where:\n"
                    + "\t<I> is the path to the original input file (graph)\n"
                    + "\t<R> is the path to the result file (with optimal solution)\n"
                    + "\t<O> is the path to the user output", Verificatore.EXIT_MISS_ARGUMENTS);
        
        
        File I = new File(args[0]);
        File R = new File(args[1]);
        File O = new File(args[2]);
        
        if(!I.exists())
            error("Input file does not exists", Verificatore.EXIT_INPUT_FILE_NOT_FOUND);
        if(!R.exists())
            error("Input file does not exists", Verificatore.EXIT_RESULT_FILE_NOT_FOUND);
        if(!O.exists())
            error("Input file does not exists", Verificatore.EXIT_OUTPUT_FILE_NOT_FOUND);
        
        new Verificatore().run(I, R, O);
    }

    public boolean verify() {
        for (int n = 0; n < this.graph.nodes.size(); n++) {
            if (!userVC[n]) {
                for (Integer x : graph.nodes.get(n).neighbour) {
                    if (!userVC[x]) {
                        System.out.print("ERRORE: non e' un vertex cover\n");
                        return false;
                    }
                }
            }
        }

        return true;
    }

    public void run(File original_graph, File optimal_result, File user_output) throws FileNotFoundException, IOException {
        BufferedReader br = new BufferedReader(new FileReader(optimal_result));

        this.OPT = Integer.parseInt(br.readLine());
        try {
            this.APX = Integer.parseInt(br.readLine());
        } catch (IOException | NumberFormatException e) {
            this.APX = this.OPT;
        }

        String optimalVC = br.readLine();
        
        br.close();
        br = new BufferedReader(new FileReader(original_graph));

        String[] line = br.readLine().split(" ");
        this.vertex = Integer.parseInt(line[0]);
        this.edges = Integer.parseInt(line[1]);

        this.graph = new Graph();

        for (int i = 0; i < edges; i++) {
            line = br.readLine().split(" ");
            graph.addEdge(Integer.parseInt(line[0]), Integer.parseInt(line[1]));
        }
        
        //System.out.println(graph.toString());

        br.close();
        br = new BufferedReader(new FileReader(user_output));

        String tmp;
        int counter = 0;
        while ((tmp = br.readLine()) != null) {
            ++counter;
            line = tmp.split(" ");
            try {
                System.out.print("Att. "+String.format("%4d",counter) );
                this.userVC = new Boolean[vertex];
                Arrays.fill(userVC, Boolean.FALSE);

                int userVCSize = Integer.parseInt(line[0]);
                System.out.print(" ["+String.format("%5d",userVCSize)+"]: ");
                if (userVCSize != line.length - 1) {
                    System.out.print("ERRORE: trovata riga incompleta\n");
                    continue;
                }
                if (userVCSize < OPT || userVCSize > APX) {
                    System.out.print("ERRORE: vertex cover minore dell'ottimo oppure eccede l'approssimazione\n");
                    continue;
                }
                
                if(userVCSize >= userBestSoFar) {
                    System.out.print("SKIP: ottimo trovato maggiore del miglior ottimo trovato");
                    continue;
                }

                for (int i = 1; i < line.length; i++) {
                    this.userVC[Integer.parseInt(line[i])] = Boolean.TRUE;
                }

                if (verify()) {
                    this.userBestSoFar = userVCSize;
                    if(userVCSize==this.OPT) {
                        System.out.println("OPTIMAL SOLUTION FOUND");
                        error("OPTIMAL SOLUTION FOUND", Verificatore.EXIT_FOUND_OPTIMAL_SOLUTION);
                    }
                    System.out.print("VALID "+(userVCSize==this.OPT?"OPT":"APX")+"\tNEW USER_OPT_SO_FAR: "+userVCSize+"\n");
                }

            } catch (Exception ex) {
                System.err.print("EXCEPTION "+ex.getClass()+"\t"+ex.getMessage()+"\n");
                Logger.getLogger(Verificatore.class.getName()).log(Level.SEVERE, null, ex);
            } 
            
        }
        
        System.out.println("\n------------------------------------------------------\nValutazione OptimalVC (Certificato)");
        tmp = optimalVC;
        line = tmp.split(" ");
            try {
                System.out.println("Optimal VC: "+Arrays.toString(line));
                this.userVC = new Boolean[vertex];
                Arrays.fill(userVC, Boolean.FALSE);

                int userVCSize = Integer.parseInt(line[0]);
                System.out.print(" ["+String.format("%5d",userVCSize)+"]: ");
                if (userVCSize != line.length - 1) {
                    System.out.print("ERRORE: trovata riga incompleta\n");
                    System.exit(1);
                }
                if (userVCSize < OPT || userVCSize > APX) {
                    System.out.print("ERRORE: vertex cover minore dell'ottimo oppure eccede l'approssimazione\n");
                    System.exit(2);
                }
                
                if(userVCSize >= userBestSoFar) {
                    System.out.print("SKIP: ottimo trovato maggiore del miglior ottimo trovato\n");
                    System.exit(3);
                }

                for (int i = 1; i < line.length; i++) {
                    this.userVC[Integer.parseInt(line[i])] = Boolean.TRUE;
                }

                if (verify()) {
                    this.userBestSoFar = userVCSize;
                    if(userVCSize==this.OPT) {
                        System.out.println("OPTIMAL SOLUTION FOUND");
                    }
                }

            } catch (Exception ex) {
                System.err.print("EXCEPTION "+ex.getClass()+"\t"+ex.getMessage()+"\n");
                Logger.getLogger(Verificatore.class.getName()).log(Level.SEVERE, null, ex);
            }

    }
}
