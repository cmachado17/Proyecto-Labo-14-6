#include <iostream>
#include "ConsumoPlatillo.h"
#include "Platillo.h"
#include "ProductoStock.h"
#include "ProductosxPlatillo.h"
#include "FuncionesGlobales.h"
#include "Usuario.h"
using namespace std;

string ConsumoPlatillo::toString()
{
    string cadena;
    cadena = "Platillo: " + mostrarNombrePlatillo(idPlatillo) + " " " " + " Fecha consumo: " + getFechaConsumo().toString();
    return cadena;
}


bool ConsumoPlatillo::LeerDeDisco(int pos)
{
    FILE *p;
    int leyo;
    p=fopen("ConsumosPlatillos.dat", "rb");
    if(p==NULL)
    {
        cout<<"El archivo no pudo abrirse"<<endl;
        return false;
    }
    fseek(p, pos*sizeof(ConsumoPlatillo),0);
    leyo=fread(this,sizeof(ConsumoPlatillo),1,p);

    fclose(p);

  return leyo;
}

bool ConsumoPlatillo::GrabarEnDisco()
{
    FILE *p;
    p=fopen("ConsumosPlatillos.dat", "ab");
    if(p==NULL)
    {
        cout<<"El archivo no pudo abrirse"<<endl;
       return false;
    }
    int escribio=fwrite(this, sizeof(ConsumoPlatillo),1,p);
    fclose(p);

    return escribio;
}



//METODO GUARDAR EN DISCO QUE PERMITE GUARDAR UNA MODIFICACION
//HAY QUE PASARLE LA POSICION Y EL MODO LECTURA VA RB+
bool ConsumoPlatillo::ModificarArchivo(int pos)
{
    FILE *p;
    p=fopen("ConsumosPlatillos.dat", "rb+");
    if(p==NULL)
    {
        cout<<"El archivo no pudo abrirse"<<endl;
       return false;
    }
    fseek(p, pos*sizeof(ConsumoPlatillo),0);
    int escribio=fwrite(this, sizeof(ConsumoPlatillo),1,p);
    fclose(p);

    return escribio;
}


/// Funciones globales para gestionar el consumo de un platillo
bool nuevoConsumoPlatillo()
{
    ConsumoPlatillo reg;
    reg = cargarConsumoPlatillo();

    bool ok = reg.GrabarEnDisco();
    if(ok){
        int platillo= reg.getIdPlatillo();
        ok = retirarProductoDelStockDesdePlatillo(platillo);
        //retirarProductoPorConsumo(platillo);
      }
    return ok;
}

int CantidadRegistrosConsumoPlatillo()
{
    FILE *p;
    p=fopen("ConsumosPlatillos.dat", "rb");
    if(p==NULL)
    {
        return 0;
    }
    size_t bytes;
    int cantidad;

    fseek(p, 0, SEEK_END);
    bytes=ftell(p);

    fclose(p);
    cantidad = bytes/sizeof(ConsumoPlatillo);
    return cantidad;
}

ConsumoPlatillo cargarConsumoPlatillo()
{
    int id;

    cout << "Ingrese el id del Platillo: ";
    cin >> id;
    cout<<endl;

    //validamos que exista ese platillo
    while(buscarPlatillo(id) == false){
       cout << "Platillo inexistente, ingrese otro: ";
       cin >> id;
       cout<<endl;
    }

    while(validarExistenciaDeProductos(id)== false){
        cout << "Lo sentimos, no hay stock de productos para cosumir ese platillo o no existe, ingrese otro: ";
        cin >> id;
        cout<<endl;
    }

    ConsumoPlatillo reg;
    Fecha actual;
    reg.setIdPlatillo(id);
    reg.setFechaConsumo(actual);
    cout<<endl;
    cout<<endl;
    system("pause");
    return reg;
}

bool validarExistenciaDeProductos(int idPlatillo)
{
    bool bandera = true;
    int *vProductos;
    int cantReg = CantidadRegistrosProductosxPlatillos();
    vProductos = new int [cantReg];
     if(vProductos==NULL){return false;}

    if(buscarPlatillo(idPlatillo))
    {
     //llamamos a la funcion para que cargue todos los productos de ese platillo
     buscarProductosXPlatillo(idPlatillo,vProductos,cantReg);

      for(int i=0; i<cantReg; i++)
      {
        //porque algunas posiciones del vector estan vacias, solo va a buscar las que tengan id de productos.

        if(vProductos[i]>0)
        {

            if(consultarStock(vProductos[i]) == 0)
            {
                bandera = false;
            }
        }
      }
    } else {
     bandera = false;
    }
      delete vProductos;
      return bandera;
}

void listarConsumosPlatillo()
{
    ConsumoPlatillo aux;
    int cantConsumoPlatillo = CantidadRegistrosConsumoPlatillo();
    cout << "LISTADO DE CONSUMOS PLATILLOS" << endl;
    cout << "----------------------------------" << endl;
    for(int i=0; i<cantConsumoPlatillo; i++)
    {
        aux.LeerDeDisco(i);
        cout<<aux.toString()<<endl;
    }
    cout<<endl;
    cout<<endl;
    system("pause");
}

    void sugerenciasXOrientacion(){
      int orientacion, dni;

      cout << "Ingrese el dni: ";
      cin >> dni;
      cout<<endl;

        while(validarUsuarioExistente(dni) == false){
            cout << "El usuario ingresado no existe en el sistema, ingrese otro DNI:  ";
            cin >> dni;
        }

         orientacion = buscarOrientacion(dni);
         buscarPlatillosXOrientacion(orientacion);
    }

     int buscarOrientacion(int dni){
        Usuario aux;
        int cantUsuarios=CantidadRegistrosUsuario();

        for(int i=0; i<cantUsuarios; i++)
        {
            aux.LeerDeDisco(i);
            if(aux.getDNI() == dni && aux.getEstadoUsuario()== true)
            {
              return aux.getIdOrientacionAlimentaria();
            }
        }

        return -1;
    }

    void buscarPlatillosXOrientacion(int orientacion){
        Platillo aux;
        int cantPlatillos = CantidadRegistrosPlatillo();
        for(int i=0; i<cantPlatillos; i++)
        {
            aux.LeerDeDisco(i);
            if(aux.getOrientacionAlimentaria() == orientacion && aux.getEstadoPlatillo() == true){
              cout<<aux.toString()<<endl;
            }
        }
        cout<<endl;
        cout<<endl;
        system("pause");
    }

    void sugerenciasXCalorias(){
      int calorias;

      cout << "Ingrese las calorias que desea consumir: ";
      cin >> calorias;
      cout<<endl;

        while(validarCalorias(calorias) == false){
            cout << "No hay platillos con esas calorias, ingrese otro valor:  ";
            cin >> calorias;
        }

        buscarPlatillosXCalorias(calorias);
    }


    void buscarPlatillosXCalorias(int calorias){
        Platillo aux;
        int cantPlatillos = CantidadRegistrosPlatillo();
        for(int i=0; i<cantPlatillos; i++)
        {
            aux.LeerDeDisco(i);
            if(aux.getCalorias() == calorias && aux.getEstadoPlatillo() == true){
              cout<<aux.toString()<<endl;
            }
        }
        cout<<endl;
        cout<<endl;
        system("pause");
    }


bool validarCalorias(int calorias)
{
    Platillo aux;
    int pos = 0;

    while(aux.LeerDeDisco(pos))
    {
        if(aux.getCalorias() == calorias && aux.getEstadoPlatillo() == true)
        {
            return true;
        }
        pos++;
    }
    return false;
}

 void menuSugerenciasPlatillos(){
   int opc;
    while(true){
        system("cls");

        cout<<"MENU SUGERENCIAS PLATILLOS"<<endl;
        cout<<"-------------------"<<endl;
        cout<<"1. SUGERENCIAS POR ORIENTACION"<<endl;
        cout<<"2. SUGERENCIAS POR CALORIAS" <<endl;
        cout<<"-------------------"<<endl;
        cout<<"0. SALIR"<<endl;
        cout<<endl;

        cout<<"OPCION: ";
        cin>>opc;

        system("cls");

        switch(opc){
            case 1: sugerenciasXOrientacion();
                    cout<<endl;
                    if(nuevoConsumoPlatillo())
                    {
                    cout<<endl;
                    cout<<"CONSUMO AGREGADO";
                    cout<<endl;
                    system("pause");
                    }
                    else
                    {
                    cout<<endl;
                    cout<<"NO SE PUDO AGREGAR EL CONSUMO";
                    cout<<endl;
                    system("pause");
                    }
                break;
            case 2:  sugerenciasXCalorias();
                      cout<<endl;
                    if(nuevoConsumoPlatillo())
                    {
                    cout<<endl;
                    cout<<"CONSUMO AGREGADO";
                    cout<<endl;
                    system("pause");
                    }
                    else
                    {
                    cout<<endl;
                    cout<<"NO SE PUDO AGREGAR EL CONSUMO";
                    cout<<endl;
                    system("pause");
                    }
                break;
            case 0: return;
                    break;
        }
        cout<<endl;
    }
  }
