# Arena Allocator

Uma implementação simples de arenas de memória em C

## como usar?

Para utilizar as arenas de memórias basta incluir o diretório **arena.h** e criar uma arena com a função **create_arena()**.

### create_arena( size_t )
  Cria uma arena de memória.
  **Recebe:** qual será a capacidade da arena em bytes
  
  **Retorna:**  uma estrutura de dados do tipo Arena.

### delete_arena( Arena* )
  Deleta uma arena de memória e devolve sua memória alocada para o sistema operacional. 
  
  **Recebe:** o ponteiro da arena a ser deletada.

### arena_alloc( Arena*, size_t )
  Aloca memória dentro de uma arena. 
  
  **Recebe:** o ponteiro da arena de memória, quantos bytes devem ser alocados.
  
  **Retorna:** um ponteiro para a memória alocada ou nulo se não foi possível alocar a memória.

### arena_free( Arena*, void* )
  Marca um bloco de memória alocada dentro da arena como livre. 
  
  **Recebe:** um ponteiro da arena e da memória a ser liberada.

### arena_copy( Arena*, void*, void*, size_t)
  Copia um bloco de memóra para dentro da arena e verifica se o bloco alocado na arena tem capacidade suficiente para receber a memória copiada.

  **Recebe:** um ponteiro para a arena, o bloco de memória dentro da arena, da memória a ser copiada e a quantidade de bytes a serem copiados.

### get_blob( void* )
  Acessa as informações de um bloco de memória alocado na arena. 
  
  **Recebe:** um ponteiro do bloco de memória.
  
  **Retorna:** as informações do bloco de memória como um ponteiro para uma estrutura de dados do tipo Blob

### set_arena_error_handling_method( ArenaErrorHandle )
  Define como as arenas deverão tratar erros

  **Recebe:** um enumerador do tipo ArenaErrorHandle que representa como os erros devem ser tratados
  
### dbg_arena( Arena* )
  Imprime as informações de uma arena no terminal. 
  
  **Recebe:** um pointeiro da arena

### dbg_blob( Arena*, void* )
  imprime as informações de um bloco de memória alocado em uma arena.

  **Recebe:** um ponteiro de arena, um ponteiro alocado na arena
