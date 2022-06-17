#include <iostream>
#include "FuncionesGlobales.h"

using namespace std;

void menuConsumos()
{
    int opc;
    while(true)
    {
        system("cls");

        cout<<"MENU CONSUMOS"<<endl;
        cout<<"-------------------"<<endl;
        cout<<"1. AGREGAR CONSUMO PRODUCTO "<<endl;
        cout<<"2. LISTAR CONSUMO PRODUCTOS"<<endl;
        cout<<"3. AGREGAR CONSUMO PLATILLO "<<endl;
        cout<<"4. LISTAR CONSUMO PLATILLOS"<<endl;
        cout<<"-------------------"<<endl;
        cout<<"0. SALIR"<<endl;
        cout<<endl;

        cout<<"OPCION: ";
        cin>>opc;

        system("cls");

        switch(opc)
        {
        case 1:
            if(nuevoConsumoProducto())
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
        case 2:
            listarConsumosPoductos();
            break;
        case 3:
            menuSugerenciasPlatillos();
            break;
        case 4:
            listarConsumosPlatillo();
            break;
        case 0:
            return;
            break;
        }
        cout<<endl;
    }
}

void menuReportes()
{
    int opc;
    while(true)
    {
        system("cls");

        cout<<"MENU REPORTES"<<endl;
        cout<<"-------------------"<<endl;
        cout<<"1. CONSUMOS ANUALES "<<endl;
        cout<<"2. CONSUMOS MENSUALES"<<endl;
        cout<<"3. PLATILLOS SUGERIDOS"<<endl;
        cout<<"4. ALERTAS DE STOCK"<<endl;
        cout<<"-------------------"<<endl;
        cout<<"0. SALIR"<<endl;
        cout<<endl;

        cout<<"OPCION: ";
        cin>>opc;

        system("cls");

        switch(opc)
        {
        case 1:
            PlatillosAnual();
            system("pause");
            break;
        case 2:
            PlatillosMensual();
            system("pause");
            break;
        case 3:
            system("pause");
            break;
        case 4:
            alertaStock();
            system("pause");
            break;
        case 0:
            return;
            break;
        }
        cout<<endl;
    }
}


string mostrarOrientacionAlimentaria(int id)
{
    OrientacionAlimentaria aux;
    int pos = 0;

    while(aux.LeerDeDisco(pos))
    {
        if (aux.getIdOrientacionAlimentaria() == id)
        {
            return aux.getDescripcion();
        }
        pos++;
    }
}

string mostrarNombreProducto(int id)
{
    Producto aux;
    int pos = 0;

    while(aux.LeerDeDisco(pos))
    {
        if (aux.getIdProducto() == id)
        {
            return aux.getNombreProducto();
        }
        pos++;
    }
}

string mostrarNombrePlatillo(int id)
{
    Platillo aux;
    int pos = 0;

    while(aux.LeerDeDisco(pos))
    {
        if (aux.getIdPlatillo() == id)
        {
            return aux.getNombrePlatillo();
        }
        pos++;
    }
}

string mayuscula(string cadena)
{
    for (int i = 0; i < cadena.length(); i++) cadena[i] = toupper(cadena[i]);
    return cadena;
}

/*Producto buscarProductoPorId(int id){
    Producto reg;
    int pos =0;

    while(reg.LeerDeDisco(pos)){
            if(reg.getIdProducto() == id && reg.getEstadoProducto()){
                return reg;
            }
        pos++;
    }
}*/

void eliminarStock(int idproducto)
{
    ProductoStock reg;
    int pos = 0;

    while(reg.LeerDeDisco(pos))
    {
        if(reg.getIdProducto() == idproducto && reg.getEstadoStock())
        {
            //EliminarProductoStock(idproducto);HACE LO MISMO QUE ESTA FUNCION RECORRE EL MISMO ARCHIVO.
            reg.setEstadoStock(false);
            reg.ModificarArchivo(pos);
        }
        pos++;
    }
}

void prueba()
{
    Producto reg;

    int pos = 0;

    while(reg.LeerDeDisco(pos))
    {
        cout << reg.toString() << endl;
        pos++;
    }
}

void PlatillosAnual()
{
    ConsumoPlatillo reg;
    int pos = 0;
    int opc;

    cout << "Ingrese el anio a consultar: ";
    cin >> opc;

    while(reg.LeerDeDisco(pos))
    {
        if (reg.getFechaConsumo().getAnio() == opc)
        {
            cout << reg.toString() << endl;
        }
        pos++;
    }
}

void PlatillosMensual()
{
    ConsumoPlatillo reg;
    int pos = 0;
    int opc;

    cout << "Ingrese el mes a consultar: ";
    cin >> opc;

    while(reg.LeerDeDisco(pos))
    {
        if (reg.getFechaConsumo().getMes() == opc)
        {
            cout << reg.toString() << endl;
        }
        pos++;
    }
}

void alertaStock()
{
    ProductoStock aux;
    int cont=0;
    int cantStocks = CantidadRegistrosStock();
    cout << "PRODUCTOS PROXIMOS A AGOTARSE" << endl;
    cout << "----------------------------------" << endl;
    for(int i=0; i<cantStocks; i++)
    {
        aux.LeerDeDisco(i);
        if(aux.getEstadoStock() &&  aux.getStock()==1)
        {
            cout<<aux.toString()<<endl;
        }
        else
        {
            cont++;
        }
    }
    cout << "----------------------------------" << endl;
    cout << "Total: " << cantStocks - cont << " registros.";
    cout<<endl;
    cout<<endl;
}

/*RetiroProducto retirarProductoPorConsumo(int idplatillo)
{
    int id;
    int dniUsuario;
    int idProducto;
    int dia, mes, anio;

    id = CantidadRegistrosRetiroProductosExistentes()+1;

    cout << "Ingrese el dni del Usuario: ";//esto despues no iria porque lo tomar�a de la sesi�n
    cin >> dniUsuario;

    while(validarUsuarioExistente(dniUsuario) == false)
    {
        cout << "El usuario ingresado no existe en el sistema, ingrese otro DNI:  ";//esto despues no iria porque lo tomar�a de la sesi�n
        cin >> dniUsuario;
    }

    cout<<endl;
    listarProductos();
    cout<<endl;
    cout<<"Ingrese el id del Producto: ";
    cin>>idProducto;

    while(validarProductoExistenteID(idProducto) == false)
    {
        cout << "El ID de producto que ingreso no existe en el sistema, ingrese otro:  ";
        cin >> idProducto;
    }

    //ver si cargamos la fecha o tomamos la actual
    cout << "Ingrese el dia: ";
    cin >> dia;
    cout << "Ingrese el mes: ";
    cin >> mes;
    cout << "Ingrese el anio: ";
    cin >> anio;

    RetiroProducto reg;

    if(retirarProductoDelStock(idProducto))
    {
        Fecha fecha(dia, mes, anio);
        reg.setIdRetiro(id);
        reg.setDniUsuario(dniUsuario);
        reg.setIdProducto(idProducto);
        reg.setFechaRetiro(fecha);
    }
    else
    {
        cout << "No hay stock del producto seleccionado" << endl;
        reg.setIdRetiro(-1);
    }

    cout<<endl;
    cout<<endl;
    return reg;
}*/
