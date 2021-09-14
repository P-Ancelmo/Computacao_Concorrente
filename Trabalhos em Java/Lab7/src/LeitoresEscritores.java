class Leitora extends Thread{
    int id;
    int i;
    Monitor monitor;

    public Leitora(Monitor monitor, int id){
        this.monitor = monitor;
        this.id = id;
    }

    private synchronized void primo(){
        try {
            this.monitor.EntraLeitor(this.id);

            if(LeitoresEscritores.variable %2 == 0 || LeitoresEscritores.variable == 1){
                System.out.println("Não é primo");
            }
            else{
                for(i=3 ; i < LeitoresEscritores.variable; i++ ){
                    if(LeitoresEscritores.variable%i == 0){
                        System.out.println("Não é primo");
                        break;
                    }
                }
                if(i == LeitoresEscritores.variable)
                    System.out.println("É primo");
            }
            this.monitor.SaiLeitor(this.id);
        }catch (Exception e){}

    }

    public void run(){
        //while(true)
        primo();
    }
}

class Escritora extends Thread{
    int id;
    Monitor monitor;

    public Escritora(Monitor monitor, int id){
        this.monitor = monitor;
        this.id = id;
    }

    private synchronized void escreve(){
        try {
            this.monitor.EntraEscritor(this.id);
            LeitoresEscritores.variable = this.id;
            this.monitor.SaiEscritor(this.id);
        }catch (Exception e){}

    }

    public void run(){
        //while(true)
        escreve();
    }
}

class LeitoraEscritora extends Thread{

    Monitor monitor;
    int id;

    public LeitoraEscritora(Monitor monitor, int id){
        this.monitor = monitor;
        this.id = id;
    }

    private synchronized void par(){
        try {
            this.monitor.EntraLeitor(this.id);
            if(LeitoresEscritores.variable%2 == 0){
                System.out.println("É par");
            }
            else{
                System.out.println("É impar");
            }
            this.monitor.SaiLeitor(this.id);

            this.monitor.EntraEscritor(this.id);
            LeitoresEscritores.variable *= 2;
            this.monitor.SaiEscritor(this.id);
        }catch (Exception e){ }

    }
    public void run(){
        par();
    }
}


public class LeitoresEscritores {
    static int variable = 0; /// váriavel que representa nossa base de dados
    static final int L = 4;  /// número de threads leitoras
    static final int E = 4;  /// número de threads escritoras
    static final int LE = 4; /// número de threads leitoras e escritoras

    public static void main(String[] args){
        Monitor monitor = new Monitor();
        Leitora[] leitoras = new Leitora[L];
        Escritora[] escritoras = new Escritora[E];
        LeitoraEscritora[] leitoraEscritoras = new LeitoraEscritora[LE];


        for(int i = 0; i < L; i++){
            leitoras[i] = new Leitora(monitor, i);
            leitoras[i].start();
        }

        for(int i = 0; i<E; i++){
            escritoras[i] = new Escritora(monitor, i);
            escritoras[i].start();
        }
        for(int i = 0; i<LE;i++){
            leitoraEscritoras[i] = new LeitoraEscritora(monitor,i);
            leitoraEscritoras[i].start();
        }
    }
}
