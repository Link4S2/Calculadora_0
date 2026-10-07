#include <Arduino.h>

typedef unsigned int num_N;//una variable entero si singo(Naturales)

typedef struct num_Z//un struct para tener numeros en Z
{
    num_N magnitud;
    num_N signo;//0 = -; 1 = +;
}num_Z;

typedef struct num_Q//un struct para tener numeros en fracciones reducidas lo más posibles
{
    num_N numerador;//numerador(arriba)
    num_N denominador;//denominador(abajo)
    num_N signo;//0 = -; 1 = +;
}num_Q;

typedef struct num_R//un struct para tener numeros reales(decimales)
{
    num_Z valor;//valor sin decimal
    num_N pos_decimal;//cantidad de digitos despues del punto
}num_R;

//----------FUNCIONES PARA N------------

//función resta de N(a-b)
num_N rest_N(num_N a, num_N b)
{
    if(a<b)//verificamos que a sea mayor que b, para que no quedemos en bucle infinito, y además, porque saldría negativo
    {
        return 0;
    }
    //se debe a que a-b = c, entonces a = b + c, entonces buscamos un número que sume a b que sea igual a a
    num_N c = 0;
    while (!(b + c == a))
    {
        c++;
    }
    return c;
}

//función multiplicación de N(a*b)
num_N mult_N(num_N a, num_N b)
{
    num_N resultado = 0;
    if(a>=b)//obtimizamos la cantidad de sumas, eligiendo el número menor como la cantidad de veces para sumar
    {
        for (num_N i = 0; i < b; i++)
        {
            resultado+=a;
        }
    }
    else
    {
        for (num_N i = 0; i < a; i++)
        {
            resultado+=b;
        }
    }
    return resultado;
}

//función division de N(a/b)(variable donde guardar residuo)
num_N div_N(num_N a, num_N b, num_N *r)
{
    num_N resultado = 0;
    num_N i = 1;
    num_N aux1 = b;
    num_N aux2 = 1;

    if(b == 0)//verificamos que b sea diferente de 0
    {
        return 0;
    }
    else
    {
        while (a>=b)//mientras que a>=b, dividimos
        {
            aux2 = mult_N(aux1, 2);//obtenemos el multiplo siguiente
            while (a >= aux2)
            {
                i = mult_N(i,2);//obtenemos i = 2^n , donde n es la cantidad de veces que recorrimos el bucle
                aux1 = aux2;//obtenemos b*i
                aux2 = mult_N(aux1, 2);
            }//mientras que a sea mayor que un multiplo de b 

            resultado += i;//sumamos el multiplo de b
            a = rest_N(a, aux1);//restamos
            //regresamos a los valores
            aux1 = b;
            i = 1;
        }
    }
    
    *r = a;
    return resultado;
}

//función potencia de N (a^b)
num_N pot_N(num_N a, num_N b)
{
    num_N resultado = 1;

    for (num_N i = 0; i < b; i++)//multiplicamos b veces
    {
        resultado = mult_N(resultado,a);//resultado = resultado*a, es decir a^b
    }
    return resultado;
}

//funcion raiz que devuelve un N (a^(1/2)) superior
num_N raiz_N(num_N a)
{
    num_N resultado = 0;
    num_N i = 0;//contador

    if(a>0)//dominio de raiz
    {
        while(mult_N(i,i)<a)//buscamos un numero que multiplicado a sí mismo dé un número mayor o igua a las raices notables, es decir un sqrt(a)pertenece a N
        {
            i++;
        }
        resultado = i;
    }
    return resultado;
}

//-----------------FUNCIONES PARA Z-------------

//función suma para Z (a+b)
num_Z sum_Z(num_Z a, num_Z b)
{
    num_Z resultado;

    if (a.signo == b.signo)//si son el mismo signo, entonces sumar magnitudes
    {
        resultado.magnitud = a.magnitud+b.magnitud;
        resultado.signo = a.signo;
    }
    else
    {
        if(a.magnitud>=b.magnitud)//si son diferente signo, restar magnitudes
        {
            resultado.magnitud = rest_N(a.magnitud, b.magnitud);
            resultado.signo = a.signo;
        }
        else
        {
            resultado.magnitud = rest_N(b.magnitud, a.magnitud);
            resultado.signo = b.signo;
        }
    }
    return resultado;
}

//función resta para Z (a-b)
num_Z rest_Z(num_Z a, num_Z b)
{
    //solo cambiamos de signo a b y sumamos
    if(b.signo == 0)
    {
        b.signo = 1;
    }
    else
    {
        b.signo = 0;
    }

    return sum_Z(a,b);
}

//función multiplicación para Z (a*b)
num_Z mult_Z(num_Z a, num_Z b)
{
    num_Z resultado;
    resultado.magnitud = mult_N(a.magnitud,b.magnitud);//multiplicamos las magnitudes

    if (a.signo == b.signo)//multiplicamos los signos, +*+=+; -*-=+
    {
        resultado.signo = 1;
    }
    else//+*-=-; -*+=-
    {
        resultado.signo = 0;
    }
    
    return resultado;
}

//función division para Z (a/b)(variable donde guardar residuo)
num_Z div_Z(num_Z a, num_Z b, num_N *r)
{
    num_Z resultado;
    resultado.magnitud = div_N(a.magnitud,b.magnitud, &*r);//dividimos las magnitudes

    if (a.signo == b.signo)//dividimos los signos, +/+=+; -/-=+
    {
        resultado.signo = 1;
    }
    else//+/-=-; -/+=-
    {
        resultado.signo = 0;
    }
    
    return resultado;
}


//funcion potencia para Z elevado a la N(a^b)
num_Z pot_Z(num_Z a, num_N b)
{
    num_Z resultado;

    resultado.magnitud = pot_N(a.magnitud, b);

    num_N r;
    num_N cal_div = div_N(b, 2, &r);
    //buscamos el signo del resultado
    if (r == 0)// si b es par, siempre será positivo el resultado
    {
        resultado.signo = 1;
    }
    else// si es impar, entonces si a = -, entonces resultado = -, si a = +, entonces resultado = +
    {
        if (a.signo == 0)
        {
            resultado.signo = 0;
        }
        else
        {
            resultado.signo = 1;
        }
    }
    
    return resultado;    
}

//---------------FUNCIONES PARA Q--------------

//funcion auxiliar para simplificar la expresion a/b a una fracion(N y D pertenecen a N y a y b pertenecen a R)
num_Q simplificar_Q(num_R a, num_R b)
{
    num_Q resultado;
    if(b.valor.magnitud == 0 || a.valor.magnitud == 0)
    {
        return {0,0,1};
    }
    if(a.pos_decimal > b.pos_decimal)//para igualar los posdecimales
    {
        b.valor.magnitud = mult_N(b.valor.magnitud, pot_N(10, (a.pos_decimal-b.pos_decimal)));
    }
    else if (a.pos_decimal < b.pos_decimal)
    {
        a.valor.magnitud = mult_N(a.valor.magnitud, pot_N(10, (b.pos_decimal-a.pos_decimal)));
    }

    num_N x = a.valor.magnitud;
    num_N y = b.valor.magnitud;
    num_N r;

    while (x != y)
    {
        if (x > y)
        {
            x = rest_N(x,y);
        }
        else
        {
            y = rest_N(y,x);
        }
    }
    
    b.valor.magnitud = div_N(b.valor.magnitud, x , &r);
    a.valor.magnitud = div_N(a.valor.magnitud, x , &r);

    resultado.numerador = a.valor.magnitud;
    resultado.denominador = b.valor.magnitud;
    resultado.signo = mult_Z({1, a.valor.signo},{1, b.valor.signo}).signo;
    return resultado;
}

//---------------FUNCIONES PARA R--------------

//funcion suma para R (a+b)
num_R sum_R(num_R a, num_R b)
{
    num_R resultado;

    resultado.pos_decimal= a.pos_decimal;//si tienen misma pos_deci

    if (a.pos_decimal > b.pos_decimal)//si la pos_deci de a es mayor que b
    {
        b.valor.magnitud = mult_N(b.valor.magnitud ,pot_N(10, rest_N(a.pos_decimal, b.pos_decimal)));//le aumento la cantidad de 0 del menor en la diferencia de pos_deci
        //por ejemplo, de 2,1(menor posi)+1,123(mayos posi)=21+1123-> 2100+1123=2.100+1.123
        resultado.pos_decimal= a.pos_decimal;
    }
    else if (a.pos_decimal < b.pos_decimal)//si la pos_deci de b es mayor que a
    {
        a.valor.magnitud = mult_N(a.valor.magnitud, pot_N(10, rest_N(b.pos_decimal, a.pos_decimal)));//igual que arriba
        resultado.pos_decimal= b.pos_decimal;
    }

    resultado.valor = sum_Z(a.valor, b.valor);

    return resultado;
}

//funcion resta para R (a-b)
num_R rest_R(num_R a, num_R b)
{
    //solo cambiamos de signo a b y sumamos
    if(b.valor.signo == 0)
    {
        b.valor.signo = 1;
    }
    else
    {
        b.valor.signo = 0;
    }

    return sum_R(a,b);
}

//funcion multiplicar para R (a*b)
num_R mult_R(num_R a, num_R b)
{
    num_R resultado;
    
    resultado.valor = mult_Z(a.valor, b.valor);//multiplicamos el valor sin decimal
    resultado.pos_decimal = a.pos_decimal + b.pos_decimal;//sumamos sus pos_deci
    //porque los R, son valor*10^pos, entonces podemos separar en valor1*valor2*(10^pos1)*(10^pos2) = valor1*valor2*10^(pos1+pos2)

    return resultado;
}

//funcion dividir para R (a/b)
num_R div_R(num_R a, num_R b)
{
    num_R resultado;
    num_N resolucion = 9;//cantidad de decimales del resultado(+resolucion, + precision)
    num_N i = 0;
    num_N r;//optimizamos calculando decimal por decimal usando el residuo para cada division

    //obtenemos la division, nos da, la parte entera y el residuo
    resultado.valor.magnitud = div_N(a.valor.magnitud, b.valor.magnitud, &r);
    if(b.valor.magnitud!=1)
    {
        while (i<resolucion)
        {
            if(r==0) break;//si el residuo es 0 entonces no hacer nada

            r = mult_N(r, 10);//a a/b le multiplico (10^r)/(10^r), entonces divido ((a*10^r)/b)*10^-r, entonces pos_deci=r
            resultado.valor.magnitud = mult_N(resultado.valor.magnitud, 10);//dezplazamos el decimal a la derecha para sumarle la parte entera de el resultado de r/b
            resultado.valor.magnitud += div_N(r, b.valor.magnitud, &r);//divido r/b y repito
            i++;
        }
    }
    
    a.pos_decimal += i;//regreso su pos_deci
    if (b.pos_decimal>=a.pos_decimal)//caso: su pos_deci de b > pos_deci de a, el pos_deci de b pasa como 10^pos_deci
    {
        resultado.pos_decimal = 0;
        resultado.valor.magnitud = mult_N(resultado.valor.magnitud, pot_N(10, rest_N(b.pos_decimal, a.pos_decimal)));//como la diferencia es posi, entonces estamos añadiendo 0 a la derecha
    }
    else//para el caso contrario
    {
        resultado.pos_decimal = rest_N(a.pos_decimal, b.pos_decimal);//como la diferencia es negativa, entonces dezplazamos la coma a la izquierda, que equivale a pos_deci
    }
    //obtenemos el signo
    resultado.valor.signo = mult_Z({1, a.valor.signo}, {1, b.valor.signo}).signo;

    return resultado;
}

//funcion aux:
//funcion potencia para R (a^b) con b pertence a N
num_R aux_pot_R(num_R a, num_N b)
{
    num_R resultado;
    //separamos a (a*10^-pos)^b=a^b*10^-pos*b
    resultado.valor = pot_Z(a.valor, b);
    resultado.pos_decimal = mult_N(a.pos_decimal, b);
    
    return resultado;
}

//Función aux para pot_R
//función raiz enesima para R (a^(1/b))con b pertenece a Z
num_R raiz_enesima_R(num_R a, num_Z b)
{
    num_R resultado;
    num_R x0 = a;//creamos el x0=a, para que esté, relativamente cerca de a^1/b
    num_N i = 1;
    //variables auxiliares
    num_R aux1;
    num_R aux2;



    if(b.signo==0)//si b es negativo, se simplifica el x0^-b a x0^b.magnitud
    {
        x0.pos_decimal = a.pos_decimal + 4;
        while (i<=10)
        {
            aux1 = mult_R(a,aux_pot_R(x0, b.magnitud));
            aux2 = rest_R({b,0},{{1,1},0});
            aux1 = sum_R(aux2, aux1);
            aux2 = mult_R(x0,aux1);
            x0 = div_R(aux2,{b,0});

            i++;
        }
    }
    else
    {
        while (i<=10)
        {
            aux1 = aux_pot_R(x0, b.magnitud);
            aux2 = div_R({{1,1},0}, aux1); 
            aux1 = mult_R(a, aux2);
            aux2 = rest_R({b,0},{{1,1},0});
            aux1 = sum_R(aux2, aux1);
            aux2 = mult_R(x0,aux1);
            x0 = div_R(aux2,{b,0});

            i++;
        }
    }
    
    return x0;
}

//------------INPUT/OUTPUT------------
void out_num_R(num_R a)
{
    char c;
    num_N z;
    num_N i;
    z = div_N(a.valor.magnitud,pot_N(10,a.pos_decimal),&i);
    c = a.valor.signo==1?'+':'-';

    printf("%c%d.%0*d\n",c,z,a.pos_decimal,i);
}

void setup()
{

}

void loop()
{

}
