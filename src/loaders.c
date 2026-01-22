#include <softberg/softberg.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>


int digits_only(const char *s) {
    while (*s) if (isdigit(*s++) == 0) return 0;
    return 1;
}

sb_vec3f parse_obj_vec3f(char buf[]) {
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

sb_vec2f parse_obj_vec2f(char buf[]) {
  sb_vec2f ret = {0};

  char *word = strtok(buf, " ");
  word = strtok(NULL, " ");
  ret.x = atof(word);

  word = strtok(NULL, " ");
  ret.y = atof(word);

  return ret;
}


sb_mesh sb_load_obj(const char * restrict path) {
  FILE *f = fopen(path, "r");
  if (!f) {errno = ENOENT; return (sb_mesh){0};}

  sb_uint num_verts = 0;
  sb_uint num_faces = 0;
  char buf[256];

  sb_uint words = 0;
  while (fscanf(f, "%s", buf) != EOF) {
    words++;
    if (strcmp(buf, "v") == 0) num_verts++;
    else if (strcmp(buf, "f") == 0) {
      num_faces+=words-3;
    }
    if (isalpha(buf[0])) words = 0;
  }
  num_faces += 3; // For some reason its always off by three, gotta investigate someday

  sb_vertex *vertices = malloc(num_verts * sizeof(sb_vertex));
  if (!vertices) {errno = ENOMEM; return (sb_mesh){0};}
  sb_vec3i *faces = malloc(num_faces * sizeof(sb_vec3i));
  if (!faces) {errno = ENOMEM; return (sb_mesh){0};}

  sb_uint line = 0;
  fseek(f, 0L, SEEK_SET);
  char *word;
  int tmp_indices[3];
  sb_uint num_indices = 0;
  sb_uint start_face_idx = 0;

  sb_uint vert1_idx = 0;
  sb_uint vert2_idx = 0;
  sb_uint vert3_idx = 0;
  sb_uint face_idx = 0;
  while (fgets(buf, 256, f)) {
    line++;
    buf[strlen(buf)-1] = '\0'; // strip newline
    switch (buf[0]) {
      case '#': break;
      case 'v':
        switch (buf[1]) {
          case 't':
            vertices[vert1_idx].texture_coordinate = parse_obj_vec2f(buf);
            vert1_idx++;
            break;
          case 'n':
            vertices[vert2_idx].normal = parse_obj_vec3f(buf);
            vert2_idx++;
            break;
          case 'p':
            printf("WARNING: %s:%i: parameter space vertices are not implemented\n", path, line);
            break;
          default:
            vertices[vert3_idx].position = parse_obj_vec3f(buf);
            vert3_idx++;
            break;
        }
        break;
      case 'f':
        word = strtok(buf, " ");
        start_face_idx = face_idx;
        while (word != NULL) {
          word = strtok(NULL, " ");
          if (word == NULL) break;

          tmp_indices[num_indices++] = atoi(word);
          if (num_indices == 3) {
            assert(face_idx < num_faces);
            if (face_idx == start_face_idx) faces[face_idx++] = (sb_vec3i) {tmp_indices[0]-1, tmp_indices[1]-1, tmp_indices[2]-1};
            else if (face_idx > start_face_idx) faces[face_idx++] = (sb_vec3i) {tmp_indices[1]-1, tmp_indices[2]-1, tmp_indices[0]-1};
            tmp_indices[0] = tmp_indices[0];
            tmp_indices[1] = tmp_indices[2];
            num_indices = 2;
          }
        }
        num_indices = 0;
        break;
        
      default: break;
    }
  }

    for (sb_uint i = 0; i < num_verts; i++) {
		vertices[i].color = (sb_color) {rand() % 255, rand() % 255, rand() % 255, 255};
	}

  fclose(f);
  return (sb_mesh) {
    vertices,
    faces,
    num_faces,
  };
}
