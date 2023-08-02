#include "hash_probing.h"
#include <assert.h>
#include <string.h>

#define CAPACIDAD_INICIAL 8


/**
 * Test to hash table
*/

/**
 * Hash function from Kernighan & Ritchie in "The C
 * Programming Language (Second Ed.)".
 */
unsigned KRHash(char *s) {
  unsigned hashval;
  for (hashval = 0; *s != '\0'; ++s) {
    hashval = *s + 31 * hashval;
  }
  return hashval;
}


/**
 * Contacto
*/
typedef struct {
  char *nombre;
  char *tel;
  unsigned int edad;
} Contacto;


/**
 * Crea un nuevo contacto.
 */
Contacto *contacto_crear(char *nombre, char *telefono, unsigned int edad) {
  Contacto *nuevo = malloc(sizeof(Contacto));
  assert(nuevo != NULL);

  nuevo->nombre = malloc(sizeof(char) * (strlen(nombre) + 1));
  assert(nuevo->nombre != NULL);
  strcpy(nuevo->nombre, nombre);

  nuevo->tel = malloc(sizeof(char) * (strlen(telefono) + 1));
  strcpy(nuevo->tel, telefono);

  nuevo->edad = edad;

  return nuevo;
}


/**
 * Crea una copia física del contacto.
 */
Contacto *contacto_copia(Contacto *contacto) {
  Contacto *copia =
      contacto_crear(contacto->nombre, contacto->tel, contacto->edad);
  return copia;
}


/**
 * Retorna 0 si los contactos tienen el mismo nombre.
 */
int contacto_comparar(Contacto *c1, Contacto *c2) {
  return strcmp(c1->nombre, c2->nombre);
}


/**
 * Función destructora de un contacto.
 */
void contacto_destruir(Contacto *contacto) {
  
  free(contacto->nombre);
  free(contacto->tel);
  free(contacto);
}


/**
 * Hashear un contacto por su nombre.
 */
unsigned contacto_heashear(Contacto *contacto) {
  return KRHash(contacto->nombre);
}


/**
 * Imprime por pantalla un contacto.
 */
void contacto_imprimir(Contacto *contacto) {
  printf("{%s, %s, %d}", contacto->nombre, contacto->tel, contacto->edad);
}


/**
 * Caso de prueba: table hash para contactos
 */
int main() {


  // Iniciar table hash
  Hash table = hash_create(CAPACIDAD_INICIAL, LINEAR,
    (FunctionCopy)contacto_copia, (FunctionDestroy)contacto_destruir, 
    (FunctionCompare)contacto_comparar, (FunctionVisit)contacto_imprimir, 
    (FunctionHash)contacto_heashear);


  // Contactos
  Contacto *agenda[10];
  agenda[0] = contacto_crear("Pepe Argento", "341 1234567", 61);
  agenda[1] = contacto_crear("Moni Argento", "341 1234567", 60);
  agenda[2] = contacto_crear("Coqui Argento", "341 1234567", 32);
  agenda[3] = contacto_crear("Paola Argento", "341 1234567", 29);
  agenda[4] = contacto_crear("Maria Elena Fuseneco", "341 1234567", 59);
  agenda[5] = contacto_crear("Dardo Fuseneco", "341 1234567", 64);
  agenda[6] = contacto_crear("Alan", "341 12312312", 22);
  agenda[7] = contacto_crear("Lucho", "341 1241242", 30);
  agenda[8] = contacto_crear("Agustin", "341 234234", 40);
  agenda[9] = contacto_crear("Martin", "341 4534541", 50);


  printf("\nCapacidad de table: %i\n", hash_capacity(table));


  // Insertar
  printf("Insercion:\n");
  for (int i = 0; i < 10; ++i) {
    printf("Insertando el contacto: ");
    contacto_imprimir(agenda[i]);
    printf(" en la casilla %d.\n",
           contacto_heashear(agenda[i]) % hash_capacity(table));
    int nElems = hash_stuffed(table);
    hash_add(table, agenda[i]);
    if (hash_stuffed(table) == nElems)
      printf("\tInsercion fallida: Colision.\n");
    else
      printf("\tInsercion exitosa.\n");
  }  

  printf("\nCapacidad de tabla: %i\n", hash_capacity(table));

  

  // Imprimir table
  hash_print(table);

  // Buscar
  printf("\nBusqueda:\n");
  for (int i = 0; i < 7; ++i) {
    printf("Buscando el contacto: ");
    contacto_imprimir(agenda[i]);
    puts("");
    Contacto *ret = hash_search(table, agenda[i]);
    if (ret != NULL)
      printf("\tSe encuentra en la table.\n");
    else
      printf("\tNo se encuentra en la table.\n");
  }

  
  // Eliminar
  printf("\nEliminacion:\n");
  for (int i = 0; i < 4; ++i) {
    printf("Eliminando el contacto: ");
    contacto_imprimir(agenda[i]);
    puts("");
    hash_delete(table, agenda[i]);
  }


  // printf("Maximo cluster: %i", max_cluster(table));


  // maria elena
  // hash_delete(table, agenda[4]);
  // hash_add(table, agenda[4]);

  // Imprimir table
  hash_print(table);

  // Buscar
  printf("\nBusqueda:\n");
  for (int i = 0; i <= 5; ++i) {
    if(i == 0 || i == 4) {
      printf("Buscando el contacto: ");
      contacto_imprimir(agenda[i]);
      puts("");
      Contacto *ret = hash_search(table, agenda[i]);
      if (ret != NULL)
        printf("\tSe encuentra en la table.\n");
      else
        printf("\tNo se encuentra en la table.\n");
    }
  }

// Insertar
  printf("Insercion:\n");
  for (int i = 2; i < 4; ++i) {
    printf("Insertando el contacto: ");
    contacto_imprimir(agenda[i]);
    printf(" en la casilla %d.\n",
           contacto_heashear(agenda[i]) % hash_capacity(table));
    hash_add(table, agenda[i]);
  }  


  // Sobrescribir un contacto
  Contacto *nuevoContacto = contacto_crear("Pepe Argento", "113 999 9999", 71);
  printf("\nSobrescribiendo el contacto: ");
  contacto_imprimir(agenda[0]);
  printf("\n\tpor: ");
  contacto_imprimir(nuevoContacto);
  puts("");
  hash_add(table, nuevoContacto);
  // Chequeamos que se haya sobrescrito
  Contacto *ret = hash_search(
      table, agenda[0]); // Es equivalente a buscar nuevoContacto porque se
                         // compara por nombre
  printf("El nuevo contacto es: ");
  if(ret != NULL)
    contacto_imprimir(ret);
  puts("");
  

  // Imprimir table
  hash_print(table);


  // Liberar memoria
  hash_destroy(table);
  for (int i = 0; i < 10; ++i)
    contacto_destruir(agenda[i]);
  contacto_destruir(nuevoContacto);

  return 0;
}