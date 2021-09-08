import java.util.Random;

class Par{
    private int par;
    public Par(){
        par = 0;
    }

    public synchronized void inc(){
        par++;
    }
    public synchronized int get(){
        return  par;
    }
}

class Tarefa extends  Thread{
    private final int id;
    private final int[] vec;
    private Par par;
    private  final int nThreads;
    public Tarefa(int[] vec, Par par, int nThreads, int id){
        this.vec = vec;
        this.par = par;
        this.nThreads = nThreads;
        this.id = id;
    }
    public void run(){
        for(int i =id; i < vec.length; i+= nThreads){
            if(vec[i]%2 == 0){
                par.inc();
            }
        }
    }
}

public class Lab6 {
    static final int N = 99999999;
    static final int nThreads = 4;
    public static void main(String[] args){
        Thread[] threads = new Thread[nThreads];
        Par par = new Par();
        Random rand = new Random();
        int[] vec = new int[N];

        //inicializa o vetor
        for(int i = 0; i < N; i++){
            vec[i] = rand.nextInt();
        }

        //cria as threads
        for(int i = 0; i < threads.length; i++){
            threads[i] = new Tarefa(vec, par, threads.length, i);
            threads[i].start(); //decidi testar iniciar a thread logo depois de instancia-la e deu certo, não sei se tem desvantagens ou se pode trazer problemas futuros
        }

        //inicia as threads
        /*for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }*/

        //aguarda o fim das threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        System.out.println("O número de pares calculado com threads é: " + par.get());
        int corretude = 0;
        for(int i =0; i < vec.length; i++){
            if(vec[i]%2 == 0)
                corretude++;
        }
        System.out.println("O número de pares calculado sequencialmente é: " + corretude);

        if(par.get() != corretude)
            System.out.println("Ops.. Algo está errado");
    }
}
