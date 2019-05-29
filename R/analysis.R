#' @export
get_analysis_group <- function() {
    group_analyses(function_definitions_analysis,
		   dynamic_calls_analysis)
}
