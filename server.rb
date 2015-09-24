# encoding: UTF-8
require 'rubygems'
require 'rack/ssl'
require 'uri'
require 'linkeddata'
require 'sinatra'
require 'sinatra/sparql'

# #TODO:0 use ssl encryption
# http://stackoverflow.com/questions/2362148
# http://stackoverflow.com/questions/3696558
# use Rack::SSL

not_found do
  status 404
  "SemIoT Gateway -- http://semiot.ru/"
end

# http://www.rubydoc.info/github/ruby-rdf/sparql#Adding_SPARQL_content_negotiation_to_a_classic_Sinatra_application
get '/' do
  settings.sparql_options.replace(standard_prefixes: true)
  repository = RDF::Repository.new do |graph|
    graph << [RDF::Node.new, RDF::DC.title, "SemIoT Gateway -- http://semiot.ru/"]
  end
  # #TODO:1 https not http:
  if params["query"]
    query = params["query"].to_s.match(/^http:/) ? RDF::Util::File.open_file(params["query"]) : ::URI.decode(params["query"].to_s)
    SPARQL.execute(query, repository)
  else
    settings.sparql_options.merge!(prefixes: {
      ssd: "http://www.w3.org/ns/sparql-service-description#",
      void: "http://rdfs.org/ns/void#"
    })
    service_description(repo: repository)
  end
end
