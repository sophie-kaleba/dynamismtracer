#' @export
get_analysis_group <- function() {
    group_analyses(function_definitions_analysis,
                   dynamic_calls_analysis)
}


#' @export
get_report_template <- function() {
    create_report_template("report", "template.Rmd",
                           package = "dynamismtracer")
}
