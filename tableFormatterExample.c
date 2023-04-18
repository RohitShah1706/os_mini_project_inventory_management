#include <stdio.h>

#include "tableFormatters/fort.h"

int main(void)
{
    ft_table_t *table = ft_create_table();

    /* Setup header */
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "N", "Driver", "Time", "Avg Speed", "Gap");

    float val = 2.23;
    // convert it to string
    char str[10];
    sprintf(str, "%f", val);
    ft_write_ln(table, "1", "Ricciardo", "1:25.945", "222.128", str);
    ft_write_ln(table, "2", "Hamilton", "1:26.373", "221.027", "+0.428");
    ft_write_ln(table, "3", "Verstappen", "1:26.469", "220.782", "+0.524");

    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    return 0;
}
