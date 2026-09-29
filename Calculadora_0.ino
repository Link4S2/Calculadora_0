#include <Arduino.h>

typedef unsigned int num_N;//una variable entero si singo(Naturales)

typedef struct num_Z//un struct para tener numeros en Z
{
    num_N signo;//0 = -; 1 = +;
    num_N magnitud;
}num_Z;

typedef struct num_Q//un struct para tener numeros en fracciones reducidas lo más posibles
{
    num_N numerador;//numerador(arriba)
    num_N denominador;//denominador(abajo)
    num_N signo;
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
    num_N dif;

    if(b == 0)//verificamos que b sea diferente de 0
    {
        return 0;
    }
    else
    {
        while ((dif=rest_N(a,b)) > 0)//mientras que la diferencia entre a y b sea mayor que cero ejecutar
        {
            a = dif;//a será la diferencia entre a y b
            resultado++;//sumar al contador en 1
        }
        if (a == b)// si resulta que el residuo es igual que b
        {
            a = 0;//restar una vez más
            resultado++;
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

//funcion auxiliar para simplificar la expresion a/b para reales
num_Q simplificar_Q(num_R a, num_R b)
{
    num_Q resultado;
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
    num_N resolucion = 4;//cantidad de decimales del resultado(+resolucion, + precision)
    num_N r;
    a.valor.magnitud = mult_N(a.valor.magnitud, pot_N(10, resolucion));//a a/b le multiplico (10^r)/(10^r), entonces divido ((a*10^r)/b)*10^-r, entonces pos_deci=r
    resultado.valor = div_Z(a.valor, b.valor, &r);//divido

    resultado.pos_decimal = 4;//regreso su pos_deci

    if (b.pos_decimal>=a.pos_decimal)//caso: su pos_deci de b > pos_deci de a, el pos_deci de b pasa como 10^pos_deci
    {
        resultado.valor.magnitud = mult_N(resultado.valor.magnitud, pot_N(10, rest_N(b.pos_decimal, a.pos_decimal)));
    }
    else//para el caso contrario
    {
        resultado.pos_decimal += rest_N(a.pos_decimal, b.pos_decimal);
    }
    
    return resultado;
}

//función raiz enesima para R (a^(1/b))
num_R raiz_enesima_R(num_R a, num_R b)
{

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
