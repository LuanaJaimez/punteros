#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

typedef struct
{
    int id;
    char marca[128];
    int anio;
    char color[51];
    float precio;
}eAuto;


int loadFromText(char* path , LinkedList* pAutos);
int mostrarAutos(LinkedList* pAutos);
//int nuevaLista(LinkedList* pAutos, LinkedList* pAutosNew);
int guardarListaCSV(char* path , LinkedList* pAutos);
int filtrarxRangoAnio(void* unAuto);
int filtrarxColor(void* unAuto);

int main()
{
    int flagTexto = 0;
    LinkedList* listaAutos = ll_newLinkedList();
    LinkedList* listaFiltrada;

    if(listaAutos == NULL)
    {
        printf("No se pudo conseguir memoria\n");
        exit(EXIT_FAILURE);
    }
    printf("Lista declarada!!!\n");




    if(!loadFromText("autos.csv", listaAutos))
    {
        printf("Autos cargados desde CSV\n");
        flagTexto = 1;
    }

    if(flagTexto == 1)
    {
        mostrarAutos(listaAutos);
    }
    else
    {
        printf("Error. No hay archivos registrados.\n");
    }

    if(flagTexto == 1)
    {
        printf("------------------------------------------------\n");
        printf("**** Autos filtrados por Anio ****\n\n");
        listaFiltrada =  ll_filter(listaAutos, filtrarxRangoAnio);
        guardarListaCSV("autos2.csv", listaFiltrada);
    }
    else
    {
        printf("Error. No hay archivos registrados.\n");
    }

    if(flagTexto == 1)
    {
        printf("------------------------------------------------\n");
        printf("**** Autos filtrados por Color ****\n\n");
        listaFiltrada =  ll_filter(listaAutos, filtrarxColor);
        guardarListaCSV("autos3.csv", listaFiltrada);
    }
    else
    {
        printf("Error. No hay archivos registrados.\n");
    }




    //Leer archivo autos csv y cargarlos en el linkedlist
    //muestro los autos
    //hacer una nueva lista con los autos que estan entre el anio 2000 2005.
    //guardar la lista filtrada en un nuevo archivo .csv

    return 0;
}

eAuto* new_auto()
{
	return (eAuto*) malloc(sizeof(eAuto));
}

int auto_setId(eAuto* this, int id)
{
	int exito = 1;

	if(this != NULL && id > 0)
	{
		this->id = id;
		exito = 0;
	}

	return exito;
}

int auto_setMarca(eAuto* this, char* marca)
{
	int exito = 1;

	if(this != NULL && marca != NULL)
	{
        //normalizeStr(marca);
        strncpy(this->marca, marca, 128);
        exito = 0;
	}

	return exito;
}

int auto_setAnio(eAuto* this, int anio)
{
	int exito = 1;

	if(this != NULL && anio > 0)
	{
		this->anio = anio;
		exito = 0;
	}

	return exito;
}

int auto_setColor(eAuto* this, char* color)
{
	int exito = 1;

	if(this != NULL && color != NULL)
	{
        //normalizeStr(color);
        strncpy(this->color, color, 51);
        exito = 0;
	}

	return exito;
}

int auto_setPrecio(eAuto* this, int precio)
{
	int exito = 1;

	if(this != NULL && precio >= 0)
	{
		this->precio = precio;
		exito = 0;
	}

	return exito;
}

int delete_auto(eAuto* this)
{
	int exito = 1;

	if(this != NULL)
	{
		free(this);
		exito = 0;
	}

	return exito;
}

eAuto* autosParametros(char* idStr, char* marcaStr, char* anioStr, char* colorStr, char* precioStr)
{
	eAuto* nAuto = NULL;
	nAuto = new_auto();

	if(precioStr != NULL && idStr != NULL && marcaStr != NULL && anioStr != NULL && colorStr != NULL)
	{

		if(auto_setId(nAuto, atoi(idStr)) == -1 ||
            auto_setMarca(nAuto, marcaStr) == -1 ||
            auto_setAnio(nAuto, atoi(anioStr)) == -1 ||
            auto_setColor(nAuto, colorStr) == -1 ||
            auto_setPrecio(nAuto, atoi(precioStr)) == -1)
		{
			delete_auto(nAuto);
			nAuto = NULL;
		}
		else
        {
            auto_setId(nAuto, atoi(idStr));
            auto_setMarca(nAuto, marcaStr);
            auto_setAnio(nAuto, atoi(anioStr));
            auto_setColor(nAuto, colorStr);
            auto_setPrecio(nAuto, atoi(precioStr));
        }
	}

	return nAuto;
}

int parser_Auto(FILE* pFile , LinkedList* pAutos)
{
	int exito = 1;
	eAuto* pAuto;
	char id[200];
	char marca[200];
	char anio[200];
	char color[200];
	char precio[200];

	if(pFile != NULL && pAutos != NULL)
	{
	    fscanf(pFile, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", id, marca, anio, color, precio);
		do
		{
			if(fscanf(pFile, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", id, marca, anio, color, precio) == 5)
			{
				pAuto = autosParametros(id, marca, anio, color, precio);

				if(pAuto != NULL)//osea si pudo cargar
				{
					ll_add(pAutos, pAuto);
					exito = 0;
				}
			}
			else
			{
				break;
			}

		}while(feof(pFile) == 0);
	}

    return exito;
}

int loadFromText(char* path , LinkedList* pAutos)
{
	FILE* pFile;
	int exito = 1;

	pFile = fopen(path, "r"); //Abre el archivo para leerlo de texto, si es que existe.

	if(pFile != NULL)
	{
		if(parser_Auto(pFile, pAutos) == 0)
		{
		    //Si todo salio bien cambia a 0
		    exito = 0;
			printf("\nLos datos de los autos en modo texto fueron cargados corretamente\n\n");
		}
		else
		{
			printf("Error al abrir el archivo\n");
		}
	}
	else
	{
		printf("Error\n");
	}

	fclose(pFile);

    return exito;
}


int auto_getId(eAuto* this, int* id)
{
	int exito = 1;

	if(this != NULL && id != NULL)
	{
		*id = this->id;
		exito = 0;
	}

	return exito;
}

int auto_getMarca(eAuto* this, char* marca)
{
	int exito = 1;

	if(this != NULL && marca != NULL)
	{
		strncpy(marca, this->marca, 128);
		exito = 0;
	}

	return exito;
}

int auto_getAnio(eAuto* this, int* anio)
{
	int exito = 1;

	if(this != NULL && anio != NULL)
	{
		*anio = this->anio;
		exito = 0;
	}

	return exito;
}

int auto_getColor(eAuto* this, char* color)
{
	int exito = 1;

	if(this != NULL && color != NULL)
	{
		strncpy(color, this->color, 51);
		exito = 0;
	}

	return exito;
}

int auto_getPrecio(eAuto* this, int* precio)
{
	int exito = 1;

	if(this != NULL && precio != NULL)
	{
		*precio = this->precio;
		exito = 0;
	}

	return exito;
}


int mostrarAuto(LinkedList* pAutos, int indice)
{
	int auxId;
	int auxAnio;
	int auxPrecio;
	char auxMarca[128];
	char auxColor[51];
	int exito = 1;

	eAuto* pAuto;
	//LinkedList* pArrayListEmployee;

	if(pAutos != NULL && indice >= 0)
	{
	    pAuto = ll_get(pAutos, indice);

		auto_getId(pAuto, &auxId);
        auto_getMarca(pAuto, auxMarca);
        auto_getAnio(pAuto, &auxAnio);
        auto_getColor(pAuto, auxColor);
        auto_getPrecio(pAuto, &auxPrecio);

        printf("%5d   %10s      %5d      %10s         %10d\n", auxId, auxMarca, auxAnio, auxColor, auxPrecio);

        exito = 0;
	}
    else if(exito == 1)
    {
        printf("Error\n");
    }

	return exito;

}

int mostrarAutos(LinkedList* pAutos)
{
	int exito = 1;

    eAuto* pAuto;


	if(pAutos != NULL)
	{
	    system("cls");
		printf("\n      ID       Marca     Anio     Color     Precio\n\n");
		for(int i = 0; i < ll_len(pAutos); i++)
		{
			pAuto = ll_get(pAutos, i);
			if(pAuto != NULL)
            {
                mostrarAuto(pAutos, i);
                exito = 0;
            }
		}
	}
	else if(exito == 1)
	{
		printf("No hay autos cargados en la lista.\n");
	}

    return exito;
}

/*
int nuevaLista(LinkedList* pAutos, LinkedList* pAutosNew)
{
    eAuto* auxAuto = NULL;
    //LinkedList* listaFiltrada = NULL;

    for(int i=0; i<ll_len(pAutos); i++)
    {
        auxAuto = (eAuto*) ll_get(pAutos, i);
        if(auxAuto != NULL)
        {
            if(auxAuto->anio >= 2000 && auxAuto->anio <= 2005)
            {
                ll_add(pAutosNew, auxAuto);
            }
        }
    }
}
*/

int guardarListaCSV(char* path , LinkedList* pAutos)
{
    int auxId;
	int auxAnio;
	int auxPrecio;
	char auxMarca[128];
	char auxColor[51];
	FILE* pFile;
	int exito = 1;

	eAuto* pAuto;
	//LinkedList* pArrayListEmployee;

	if(pAutos != NULL && path != NULL)
	{
	    pFile = fopen(path, "w");
	    if(pFile != NULL)
        {
            exito = 0;
            for(int i = 0; i < ll_len(pAutos); i++)
            {
                pAuto = ll_get(pAutos, i);

                auto_getId(pAuto, &auxId);
                auto_getMarca(pAuto, auxMarca);
                auto_getAnio(pAuto, &auxAnio);
                auto_getColor(pAuto, auxColor);
                auto_getPrecio(pAuto, &auxPrecio);

                printf("%5d   %10s      %5d      %10s         %10d\n", auxId, auxMarca, auxAnio, auxColor, auxPrecio);
            }

            fclose(pFile);

			printf("\nArchivo guardado correctamente\n\n");
        }
        else if(exito == 1)
        {
            printf("Error\n");
        }
	}

	return exito;
}


int filtrarxRangoAnio(void* unAuto)
{
    int auxReturn = 0;
    eAuto* x;

    if(unAuto != NULL)
    {
        x = (eAuto*) unAuto;
        if(x->anio >= 2000 && x->anio <= 2005)
        {
            auxReturn = 1;
        }
    }

    return auxReturn;
}


int filtrarxColor(void* unAuto)
{
    int auxReturn = 0;
    eAuto* x;

    if(unAuto != NULL)
    {
        x = (eAuto*) unAuto;
        if(strcmp(x->color, "Pink") == 0)
        {
            auxReturn = 1;
        }
    }

    return auxReturn;
}
