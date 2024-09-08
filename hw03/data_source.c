#include "data_source.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Container CSV column header
#define CONTAINER_COLUMNS_COUNT 9

#define CONTAINER_ID 0
#define CONTAINER_X 1
#define CONTAINER_Y 2
#define CONTAINER_WASTE_TYPE 3
#define CONTAINER_CAPACITY 4
#define CONTAINER_NAME 5
#define CONTAINER_STREET 6
#define CONTAINER_NUMBER 7
#define CONTAINER_PUBLIC 8

// Path CSV column header
#define PATH_COLUMNS_COUNT 3

#define PATH_A 0
#define PATH_B 1
#define PATH_DISTANCE 2

struct data_source
{
    char ***containers;
    size_t containers_count;

    char ***paths;
    size_t paths_count;
};

static struct data_source *data_source;

static char *readline(FILE *file)
{
    assert(file != NULL);

    size_t line_size = 32;
    size_t offset = 0;
    char *line = NULL;
    size_t add = 32;

    char *tmp;
    do {
        tmp = realloc(line, line_size + 1);
        if (tmp == NULL) {
            free(line);
            return NULL;
        }

        line = tmp;
        memset(line + (line_size - add), 0, add);

        if (fgets(line + offset, add + 1, file) == NULL) {
            break;
        }

        offset = line_size;
        line_size += add;

    } while (strchr(line, '\n') == NULL);

    if (ferror(file) || feof(file)) {
        free(line);
        return NULL;
    }

    line[strlen(line) - 1] = '\0';

    return line;
}

static void free_pointer_array(void **array, size_t size)
{
    assert(array != NULL);

    for (size_t index = 0; index < size; index++) {
        free(array[index]);
    }

    free(array);
}

static void free_splitted_lines(char ***lines, int column_count)
{
    assert(lines != NULL);

    for (int index = 0; lines[index] != NULL; index++) {
        free_pointer_array((void **) lines[index], column_count);
    }

    free(lines);
}

static char **split_csv_line(char *line, size_t expected_count)
{
    assert(line != NULL);

    char **splitted_line = calloc(expected_count + 1, sizeof(char*));
    if (splitted_line == NULL) {
        return NULL;
    }
    size_t line_length = strlen(line);

    char *token = strtok(line, ",");
    size_t parsed_length = 0;

    for (size_t index = 0; index < expected_count; index++) {
        if (token == NULL) {
            free_pointer_array((void **) splitted_line, index);
            return NULL;
        }

        parsed_length += strlen(token) + 1;

        splitted_line[index] = malloc(strlen(token) + 1);
        if (splitted_line[index] == NULL) {
            free_pointer_array((void **) splitted_line, index);
            return NULL;
        }

        strcpy(splitted_line[index], token);

        if (line_length > parsed_length
                && strchr(line + parsed_length, ',') == line + parsed_length) {
            token = "";
        } else {
            token = strtok(NULL, ",");
        }
    }

    if (token != NULL) {
        free_pointer_array((void **) splitted_line, expected_count);
        return NULL;
    }

    return splitted_line;
}

static char ***parse_csv(const char *path, int column_count)
{
    assert(path != NULL);

    FILE *csv_file = fopen(path, "r");

    if (csv_file == NULL) {
        return NULL;
    }

    size_t array_capacity = 8;
    size_t line_count = 0;
    char ***lines = calloc(array_capacity + 1, sizeof(char **));

    if (lines == NULL) {
        fclose(csv_file);
        return NULL;
    }

    char *line;
    void *tmp;

    while ((line = readline(csv_file)) != NULL) {
        if (line_count >= array_capacity) {
            tmp = realloc(lines, (array_capacity * 2 + 1) * sizeof(char **));
            if (tmp == NULL) {
                free(line);
                free_splitted_lines(lines, column_count);
                fclose(csv_file);
                return NULL;
            }
            lines = tmp;
            memset(lines + line_count, 0, (array_capacity + 1) * sizeof(char **));
            array_capacity *= 2;
        }

        if ((lines[line_count] = split_csv_line(line, column_count)) == NULL) {
            free(line);
            free_splitted_lines(lines, column_count);
            fclose(csv_file);
            return NULL;
        }

        line_count++;
        free(line);
    }

    if (ferror(csv_file)) {
        free_splitted_lines(lines, column_count);
        fclose(csv_file);
        return NULL;
    }

    fclose(csv_file);
    return lines;
}

static size_t count_lines(void **lines)
{
    size_t lines_count = 0;
    while (lines[lines_count] != NULL) {
        lines_count++;
    }
    return lines_count;
}

bool init_data_source(const char *containers_path, const char *paths_path)
{
    data_source = malloc(sizeof(struct data_source));
    if (data_source == NULL) {
        return false;
    }

    data_source->containers = parse_csv(containers_path, CONTAINER_COLUMNS_COUNT);
    if (data_source->containers == NULL) {
        free(data_source);
        return false;
    }
    data_source->containers_count = count_lines((void **) data_source->containers);

    data_source->paths = parse_csv(paths_path, PATH_COLUMNS_COUNT);
    if (data_source->paths == NULL) {
        free_splitted_lines(data_source->containers, CONTAINER_COLUMNS_COUNT);
        free(data_source);
        return false;
    }
    data_source->paths_count = count_lines((void **) data_source->paths);

    return true;
}

void destroy_data_source(void)
{
    free_splitted_lines(data_source->containers, CONTAINER_COLUMNS_COUNT);
    free_splitted_lines(data_source->paths, PATH_COLUMNS_COUNT);
    free(data_source);
}

const char *get_container_id(size_t line_index)
{
    if (line_index >= data_source->containers_count) {
        return NULL;
    }
    return data_source->containers[line_index][CONTAINER_ID];
}

const char *get_container_x(size_t line_index)
{
    if (line_index >= data_source->containers_count) {
        return NULL;
    }
    return data_source->containers[line_index][CONTAINER_X];
}

const char *get_container_y(size_t line_index)
{
    if (line_index >= data_source->containers_count) {
        return NULL;
    }
    return data_source->containers[line_index][CONTAINER_Y];
}

const char *get_container_waste_type(size_t line_index)
{
    if (line_index >= data_source->containers_count) {
        return NULL;
    }
    return data_source->containers[line_index][CONTAINER_WASTE_TYPE];
}

const char *get_container_capacity(size_t line_index)
{
    if (line_index >= data_source->containers_count) {
        return NULL;
    }
    return data_source->containers[line_index][CONTAINER_CAPACITY];
}

const char *get_container_name(size_t line_index)
{
    if (line_index >= data_source->containers_count) {
        return NULL;
    }
    return data_source->containers[line_index][CONTAINER_NAME];
}

const char *get_container_street(size_t line_index)
{
    if (line_index >= data_source->containers_count) {
        return NULL;
    }
    return data_source->containers[line_index][CONTAINER_STREET];
}

const char *get_container_number(size_t line_index)
{
    if (line_index >= data_source->containers_count) {
        return NULL;
    }
    return data_source->containers[line_index][CONTAINER_NUMBER];
}

const char *get_container_public(size_t line_index)
{
    if (line_index >= data_source->containers_count) {
        return NULL;
    }
    return data_source->containers[line_index][CONTAINER_PUBLIC];
}

const char *get_path_a_id(size_t line_index)
{
    if (line_index >= data_source->paths_count) {
        return NULL;
    }
    return data_source->paths[line_index][PATH_A];
}

const char *get_path_b_id(size_t line_index)
{
    if (line_index >= data_source->paths_count) {
        return NULL;
    }
    return data_source->paths[line_index][PATH_B];
}

const char *get_path_distance(size_t line_index)
{
    if (line_index >= data_source->paths_count) {
        return NULL;
    }
    return data_source->paths[line_index][PATH_DISTANCE];
}

