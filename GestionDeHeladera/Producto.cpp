#include <iostream>
#include <iomanip>
#include "Producto.h"
#include "ProductoStock.h"
#include "IngresoProducto.h"
#include "RetiroProducto.h"
#include "FuncionesGlobales.h"
#include "Usuario.h"

using namespace std;


string Producto::getNombreProducto()
{
    string nombreProductos;
    nombreProductos = nombreProducto;
    return nombreProductos;
}

string Producto::toString()
{
    string cadena;
    cadena = "Id: " + to_string(idProducto) + " " " " + " Nombre: " + nombreProducto  + " " " " + " Fecha vencimiento: " + fechaVencimiento.toString();;
    return cadena;
}

void Producto::toList()
{
    cout << left;
    cout << setw(4) << getIdProducto();
    cout << setw(15) << getNombreProducto();
    cout << setw(15)  << getFechaVencimiento().toString() << endl;
}

bool Producto::operator == (Fecha fecha)
{
    if(fechaVencimiento.getDia()== fecha.getDia() && fechaVencimiento.getMes()== fecha.getMes() && fechaVencimiento.getAnio()== fecha.getAnio())
    {
        return true;
    }
    return false;
}

bool Producto::LeerDeDisco(int pos)
{
    FILE *p;
    int leyo;
    p=fopen("Productos.dat", "rb");
    if(p==NULL)
    {
        return false;
    }
    fseek(p, pos*sizeof(Producto),0);
    leyo=fread(this,sizeof(Producto),1,p);

    fclose(p);
    return leyo;
}

bool Producto::GrabarEnDisco()
{
    FILE *p;
    p=fopen("Productos.dat", "ab");
    if(p==NULL)
    {
        cout<<"El archivo no pudo abrirse"<<endl;
        return false;
    }
    int escribio=fwrite(this, sizeof(Producto),1,p);
    fclose(p);

    return escribio;
}



//METODO GUARDAR EN DISCO QUE PERMITE GUARDAR UNA MODIFICACION
//HAY QUE PASARLE LA POSICION Y EL MODO LECTURA VA RB+
bool Producto::ModificarArchivo(int pos)
{
    FILE *p;
    p=fopen("Productos.dat", "rb+");
    if(p==NULL)
    {
        return false;
    }
    fseek(p, pos*sizeof(Producto),0);
    int escribio=fwrite(this, sizeof(Producto),1,p);
    fclose(p);

    return escribio;
}

/// Funciones globales para gestionar Producto
bool nuevoProducto()
{
    Producto reg;
    reg = cargarProducto();
    bool ok = reg.GrabarEnDisco();
    return ok;
}

int CantidadRegistrosProductos()
{
    FILE *p;
    p=fopen("Productos.dat", "rb");
    if(p==NULL)
    {
        return 0;
    }
    size_t bytes;
    int cantidad;

    fseek(p, 0, SEEK_END);
    bytes=ftell(p);

    fclose(p);
    cantidad = bytes/sizeof(Producto);
    return cantidad;
}

Producto cargarProducto()
{
    int id;
    string nombre;
    int dia, mes, anio, dniUsuario;
    bool estado = true;

    id = CantidadRegistrosProductos()+1;

    cout << "Ingrese el dni del Usuario: ";
    cin >> dniUsuario;

    while(validarUsuarioExistente(dniUsuario) == false)
    {
        cout << "El usuario ingresado no existe en el sistema, ingrese otro DNI:  ";
        cin >> dniUsuario;
    }

    cout << "Ingrese el nombre del Producto: ";
    cin.ignore();
    getline(cin, nombre);
    nombre = mayuscula(nombre);

    while(validarProductoExistente(nombre))
    {
        cout <<"Producto ya ingresado, ingrese otro: ";
        getline(cin, nombre);
        nombre = mayuscula(nombre);
    }

    /*cout << "Ingrese el dia: ";
    cin >> dia;
    cout << "Ingrese el mes: ";
    cin >> mes;
    cout << "Ingrese el anio: ";
    cin >> anio; */

    Fecha fechavencimiento(7);
    Fecha fecha;
    Producto reg;
    reg.setIdProducto(id);
    reg.setNombreProducto(nombre);
    reg.setFechaVencimiento(fechavencimiento);
    reg.setEstadoProducto(estado);

    //agrega producto al stock
    agregarProductoNuevoAlStock(id);
    //genera el ingreso
    agregarIngresoDesdeNuevoProducto(id, dniUsuario, fecha);

    cout<<endl;
    cout<<endl;
    system("pause");
    return reg;
}

bool validarProductoExistente(string n)
{
    Producto aux;
    int pos = 0;

    while(aux.LeerDeDisco(pos))
    {

        if(aux.getNombreProducto() == n && aux.getEstadoProducto() == true)
        {
            return true;
        }
        pos++;
    }
    return false;
}

//valida el id de producto
bool validarProductoExistenteID(int id)
{
    Producto aux;
    int pos = 0;

    while(aux.LeerDeDisco(pos))
    {
        if(aux.getIdProducto() == id && aux.getEstadoProducto() == true)
        {
            return true;
        }
        pos++;
    }
    return false;
}

void listarProductos()
{
    Producto aux;
    int cont=0;
    int cantProductos = CantidadRegistrosProductos();

    cout << left;
    cout << setw(20) << "\t";
    cout << "PRODUCTOS" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << setw(4)  << "ID";
    cout << setw(15) << "NOMBRE";
    cout << setw(15) << "VENCIMIENTO"<<endl;
    cout << "---------------------------------------------------------------" << endl;

    for(int i=0; i<cantProductos; i++)
    {
        aux.LeerDeDisco(i);
        if(aux.getEstadoProducto())
        {
            cout << left;
            cout << setw(4)  << aux.getIdProducto();
            cout << setw(15) << aux.getNombreProducto();
            cout << setw(15) << aux.getFechaVencimiento().toString() << endl;
        }
        else
        {
            cont++;
        }
    }
    cout << "---------------------------------------------------------------" << endl;
    cout << "Total: " << cantProductos - cont<< " Productos.";
    cout<<endl;
    cout<<endl;
}

int EliminarProducto()
{
    Producto aux;
    int pos=0, idproducto;

    listarProductos();
    cout<<endl;

    cout<<"Ingrese el ID del producto a eliminar: ";
    cin>>idproducto;

    while(aux.LeerDeDisco(pos)==1)
    {
        if(aux.getIdProducto() == idproducto && aux.getEstadoProducto() == true)
        {
            aux.setEstadoProducto(false);
            bool modifico = aux.ModificarArchivo(pos);
            if (modifico)
            {
                eliminarStock(idproducto);
                eliminarPlatillos(idproducto);
            }
            return pos;
        }
        pos++;
    }
    return -1;
}


void alertaDeProductosAVencer()
{
    Producto aux;
    Fecha fecha;
    int cont=0;
    int cantReg = CantidadRegistrosProductos();
    bool bandera = false;

    cout << "PRODUCTOS PROXIMOS A VENCER" << endl;
    cout << "----------------------------------" << endl;

    cout << left;
    cout << setw(17) << "PRODUCTO";
    cout << setw(17)  << "VENCIMIENTO" << endl;
    cout << "----------------------------------" << endl;

    for(int i=0; i<cantReg; i++)
    {
        if(aux.LeerDeDisco(i))
        {
            if(aux.getEstadoProducto() == true)
            {
                //usa la sobrecarga de operator
                if(aux == fecha)
                {
                    cout << left;
                    cout << setw(17) << aux.getNombreProducto();
                    cout << setw(17)  << aux.getFechaVencimiento().toString() << endl;
                    cont++;
                    bandera = true;
                }
            }
        }
    }
    if(bandera)
    {
        cout << "----------------------------------" << endl;
        cout << "Total: " << cont << " Productos.";
    }
    else
    {
        system("cls");
        cout << "No hay productos proximos a vencerse." << endl;
    }

    cout<<endl;
    cout<<endl;
}

void buscarProducto()
{
    Producto aux;
    int pos = 0;
    string nombre;
    bool bandera = false;

    cout << "Ingrese el nombre del Producto a buscar: ";
    cin.ignore();
    getline(cin, nombre);
    nombre = mayuscula(nombre);

    system("cls");

    cout << "PRODUCTOS" << endl;
    cout << "----------------------------------" << endl;
    cout << left;
    cout << setw(4) << "ID";
    cout << setw(15) << "PRODUCTO";
    cout << setw(15)  << "VENCIMIENTO" << endl;
    cout << "----------------------------------" << endl;

    while(aux.LeerDeDisco(pos))
    {
        if(aux.getNombreProducto() == nombre && aux.getEstadoProducto())
        {
            aux.toList();
            bandera = true;
        }
        pos++;
    }

    if(bandera == false)
    {
        system("cls");
        cout << "No hay productos con ese nombre." << endl;
    }
    cout << endl;
}

void menuProducto()
{
    int opc;
    while(true)
    {
        system("cls");

        cout<<"MENU PRODUCTO"<<endl;
        cout<<"-------------------"<<endl;
        cout<<"1. AGREGAR PRODUCTO "<<endl;
        cout<<"2. ELIMINAR PRODUCTO "<<endl;
        cout<<"3. INGRESAR PRODUCTO EXISTENTE "<<endl;
        cout<<"4. RETIRAR PRODUCTO EXISTENTE "<<endl;
        cout<<"5. LISTAR PRODUCTOS "<<endl;
        cout<<"6. LISTAR INGRESOS DE PRODUCTOS "<<endl;
        cout<<"7. LISTAR RETIROS DE PRODUCTOS "<<endl;
        cout<<"8. ALERTA DE PRODUCTOS A VENCER "<<endl;
        cout<<"9. BUSCAR PRODUCTO"<<endl;
        cout<<"-------------------"<<endl;
        cout<<"0. SALIR"<<endl;
        cout<<endl;

        cout<<"OPCION: ";
        cin>>opc;

        system("cls");

        switch(opc)
        {
        case 1:
            if(nuevoProducto())
            {
                cout<<endl;
                cout<<"PRODUCTO AGREGADO";
                cout<<endl;
                system("pause");
            }
            else
            {
                cout<<endl;
                cout<<"NO SE PUDO AGREGAR EL PRODUCTO";
                cout<<endl;
                system("pause");
            }
            break;
        case 2:
            if(EliminarProducto()!= -1)
            {
                cout<<endl;
                cout<<"PRODUCTO ELIMINADO";
                cout<<endl;
                system("pause");
            }
            else
            {
                cout<<endl;
                cout<<"NO SE PUDO ELIMINAR EL PRODUCTO";
                cout<<endl;
                system("pause");
            }
            break;
        case 3:
            if(ingresarProducto())
            {
                cout<<endl;
                cout<<"PRODUCTO AGREGADO";
                cout<<endl;
                system("pause");
            }
            else
            {
                cout<<endl;
                cout<<"NO EXISTE EL PRODUCTO";
                cout<<endl;
                system("pause");
            }
            break;

        case 4:
            if(retirarProducto())
            {
                cout<<endl;
                cout<<"PRODUCTO RETIRADO";
                cout<<endl;
                system("pause");
            }
            else
            {
                cout<<endl;
                cout<<"NO SE PUDO RETIRAR EL PRODUCTO";
                cout<<endl;
                system("pause");
            }
            break;
        case 5:
            listarProductos();
            system("pause");
            break;
        case 6:
            listarIngresosDeProductosExistentes();
            system("pause");
            break;
        case 7:
            listarRetirosDeProductosExistentes();
            system("pause");
            break;
        case 8:
            alertaDeProductosAVencer();
            system("pause");
            break;
        case 9:
            buscarProducto();
            system("pause");
            break;
        case 0:
            return;
            break;
        }
        cout<<endl;
    }
}
