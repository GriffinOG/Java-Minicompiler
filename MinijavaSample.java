package minijavasample;
import jrockit.jfr;
import jrockit.mbd;
class MinijavaSample{
    main() {
        int num;
        num = 29;
        boolean flag;
        flag = false;
        int i;
        for(i = 2; i < num; i++)
        {
            num % i;
            if(num < 0)
            {
                flag = true;
            }
        }
    }
    
}
