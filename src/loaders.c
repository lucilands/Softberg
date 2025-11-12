#include <softberg/softberg.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>


int digits_only(const char *s) {
    while (*s) if (isdigit(*s++) == 0) return 0;
    return 1;
}

sb_vec3f parse_obj_vertex(char buf[]) {
  sb_vec3f ret = {0};

  char *word = strtok(buf, " ");
  word = strtok(NULL, " ");
  ret.x = atof(word);

  word = strtok(NULL, " ");
  ret.y = atof(word);

  word = strtok(NULL, " ");
  ret.z = atof(word);

  return ret;
}

sb_vec3i parse_obj_face(char buf[]) {
  sb_vec3i ret = {0};

  // NOTE: obj stores indices that start at 1, we want them to start at 0 tho
  char *word = strtok(buf, " ");
  word = strtok(NULL, " ");
  ret.x = atoi(word) - 1;

  word = strtok(NULL, " ");
  ret.y = atoi(word) - 1;

  word = strtok(NULL, " ");
  ret.z = atoi(word) - 1;  

  return ret;
}

sb_mesh sb_load_obj(const char * restrict path) {
  FILE *f = fopen(path, "r");
  if (!f) {errno = ENOENT; return (sb_mesh){0};}

  sb_uint num_verts = 0;
  sb_uint num_faces = 0;
  char buf[256];

  while (fscanf(f, "%s", buf) != EOF) {
    if (strcmp(buf, "v") == 0) num_verts++;
    else if (strcmp(buf, "f") == 0) num_faces++;
  }

  sb_vec3f *vertices = malloc(num_verts * sizeof(sb_vec3f));
  if (!vertices) {errno = ENOMEM; return (sb_mesh){0};}
  sb_vec3i *faces = malloc(num_faces * sizeof(sb_vec3i));
  if (!faces) {errno = ENOMEM; return (sb_mesh){0};}
  sb_color *colors = malloc(num_verts * sizeof(sb_color));
  if (!colors) {errno = ENOMEM; return (sb_mesh){0};}

  sb_uint vert_idx = 0;
  sb_uint face_idx = 0;

  for (sb_uint i = 0; i < num_verts; i++) {
    colors[i] = (sb_color) {rand() % 255, rand() % 255, rand() % 255};
  }

  sb_uint line = 0;
  fseek(f, 0L, SEEK_SET);
  while (fgets(buf, 256, f)) {
    line++;
    buf[strlen(buf)-1] = '\0'; // strip newline
    switch (buf[0]) {
      case '#': break;
      case 'v':
        switch (buf[1]) {
          case 't':
            //printf("WARNING: %s:%i: texture coords are not implemented yet\n", path, line);
            break;
          case 'n':
            //printf("WARNING: %s:%i: vertex normals are not implemented yet\n", path, line);
            break;
          case 'p':
            //printf("WARNING: %s:%i: parameter space vertices are not implemented\n", path, line);
            break;
          default:
            vertices[vert_idx++] = parse_obj_vertex(buf);
        }
        break;
      case 'f':
        faces[face_idx++] = parse_obj_face(buf);
        
      default: break;
    }
  }

  fclose(f);
  return (sb_mesh) {
    vertices,
    faces,
    colors,
    num_faces,
  };
}
