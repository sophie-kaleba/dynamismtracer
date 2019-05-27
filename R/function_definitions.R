function_definitions_analysis <- create_analysis(

    id = "function_definitions",

    reducer = function(analyses) {

        if(nrow(analyses$function_definitions) == 0) {
            return(list())
        }

        list(function_definitions = analyses$function_definitions)
    },

    summarizer = function(analyses) {

        encode_sequence <- function(seq) {
            str_c("(", str_c(seq, collapse = " "), ")", sep = "")
        }

        decode_sequence <- function(seq) {
            unlist(str_split(str_sub(seq, 2, -2), " "))
        }

        function_definitions_with_script  <-
            analyses$function_definitions %>%
            group_by(function_id) %>%
            summarize(formal_parameter_count = first(formal_parameter_count),
                      package = first(package),
                      function_name = encode_sequence(unique(decode_sequence(function_name))),
                      definition = first(definition),
                      script = encode_sequence(str_c(package, script_type, script_name,
                                                     sep = "/"))) %>%
            ungroup()

        function_definitions <-
            function_definitions_with_script %>%
            select(-script)

        list(function_definitions = function_definitions,
             function_definitions_with_script = function_definitions_with_script)
    }
)
