#include <stdio.h>
#include <string.h>
#include <math.h>
#include<iostream>
#include<string>
#include <sstream>
#include <string>
using std::string;
using std::istringstream;using std::ostringstream;

#include "GraphLite.h"
#include "svd.h"
#define VERTEX_CLASS_NAME(name) svd##name


unsigned long long colNum,rowNum;

void array_assign(double *a, double *b, int len){
    for(int i = 0; i < len; i++){
        b[i] = a[i];
    }
}

class VERTEX_CLASS_NAME(InputFormatter): public InputFormatter {
public:
    
    int64_t getVertexNum() {
        unsigned long long n;
        sscanf(m_ptotal_vertex_line, "%llu", &n);
        m_total_vertex= n;
        return m_total_vertex;
    }
    int64_t getEdgeNum() {
        unsigned long long n;
        sscanf(m_ptotal_edge_line, "%llu", &n);
        m_total_edge= n;
        return m_total_edge;
    }
    int getVertexValueSize() {
        m_n_value_size = sizeof(struct message_value);
        return m_n_value_size;
    }
    int getEdgeValueSize() {
        m_e_value_size = sizeof(double);
        return m_e_value_size;
    }
    int getMessageValueSize() {
        m_m_value_size = sizeof(struct message_value);
        return m_m_value_size;
    }
     void loadGraph() 
    {
	
		unsigned long long last_vertex;
        unsigned long long from;
        unsigned long long to;
        double weight = 0;
	    double temp = 0 ; // element of matrix
        
        // double value = 1;
	    struct message_value value;  // value
	    double matrixCol[20];
	    double identityCol[20];
        int outdegree = 0;
        int count = 1; // except from and to
        const char *line= getEdgeLine();
	    string sline(line);
	    istringstream is(sline);
		printf("colNum: %llu\n", colNum);
        // Note: modify this if an edge weight is to be read
        //       modify the 'weight' variable

        sscanf(line, "%llu %llu", &from, &to);
        printf("%llu %llu",from, to);
        int i,j;
	    i = j = 0; //index of matrix column and identity column 
		//ÊäÈëÎÄŒþ£¿
	    while(is >> temp)
	    {
			if(count > 2 && i < colNum) 
			{
				identityCol[i] = temp;
				printf(" %f",temp);
				++i;

			}
			else if(count > 2 + colNum)
			{
				
				matrixCol[j] = temp;
				printf(" %f",temp);
				++j;
			}

			++count;
		}
		array_assign(matrixCol, value.matrixCol, rowNum);
		array_assign(identityCol, value.identityCol, colNum);
        addEdge(from, to, &weight);
	
        last_vertex = from;
        ++outdegree;
        for (int64_t t = 1; t < m_total_edge; ++t) 
        {
	       
			//double nmatrixCol[20];
			//double nidentityCol[20];
            line = getEdgeLine();
	        string nsline(line);
	        istringstream nis(nsline);

            // Note: modify this if an edge weight is to be read
            //       modify the 'weight' variable

            sscanf(line, "%lld %lld", &from, &to);

            printf("\n");
	        printf("%lld %lld",from,to);
	        i = j = 0; //index of matrix column and identity column
            if (last_vertex != from)
            {

                addVertex(last_vertex, &value, outdegree);
                last_vertex = from;
		        count=1;
		        while(nis >> temp)
       		    {
                	if(count > 2 && i < colNum)
               		{
						identityCol[i] = temp;
				        printf(" %f",temp);
				        ++i;
                	}
		            else if(count > 2 + colNum)
			        {
						matrixCol[j] = temp;
						++j;
						printf(" %f",temp);
			        }
		
               		++count;
       		    }
		  
				array_assign(matrixCol, value.matrixCol, rowNum);
				array_assign(identityCol, value.identityCol, colNum);
				outdegree = 1;
				printf("\nthe vertex id:");
				printf("%llu\n",last_vertex);
				printf("\n");
				for(int i = 0; i < rowNum; i++)
        		{
            		printf("%.2lf ", (value.matrixCol)[i]);
       			}
				printf("\n");
				for(int i = 0; i < colNum; i++)
        		{
            		printf("%.2lf ", (value.identityCol)[i]);
       			}
				printf("\n");
            } 
            else 
            {
                ++outdegree;
            }
            addEdge(from, to, &weight);
        }
        addVertex(last_vertex, &value, outdegree);
		printf("\nthe vertex id:");
		printf("%llu\n",last_vertex);
		printf("\n");
		for(int i = 0; i < rowNum; i++)
        {
            printf("%.2lf ", (value.matrixCol)[i]);
        }
		printf("\n");
		for(int i = 0; i < colNum; i++)
        {
            printf("%.2lf ", (value.identityCol)[i]);
        }
		printf("\n");
    }
};

class VERTEX_CLASS_NAME(OutputFormatter): public OutputFormatter {
public:
	void writeResult(){
		int64_t vid;
        struct message_value value;
        // outs: sigma; outu: U ; outv : V ; outb : A*J1*J2*...
        double outs, outu[200], outv[200], outb[200];
        char s[1024];
		
        for (ResultIterator r_iter; ! r_iter.done(); r_iter.next() )
        {
            r_iter.getIdValue(vid, &value);
			// get array b
            for(int i = 0; i < rowNum; i++)
            {
                outb[i] = value.matrixCol[i];
            }
			// print array b 
			for(int i = 0; i < rowNum; i++)
            {
                printf("%.2lf ", outb[i]);
            }
			printf("\n");
			// get array v
            for(int i = 0; i < colNum; i++)
            {
                outv[i] = value.identityCol[i];
            }
			// print array v
			for(int i = 0; i < colNum; i++)
            {
                printf("%.2lf ", outv[i]);;
            }
			// compute the sigma and array u
            normalize(outb, outu, &outs, rowNum);
			// write step
            int offset = 0;
			// write array u
            for(int i = 0; i < rowNum; i++)
            {
                offset += sprintf(s+offset, "%.2lf ", outu[i]);
            }
			offset += sprintf(s+offset, "   ");
			// write sigma
            offset += sprintf(s+offset, "%.2lf", outs);
			offset += sprintf(s+offset, "    ");
			// write array v
            for(int i = 0; i < colNum; i++)
            {
                offset += sprintf(s+offset, "%.2lf ", outv[i]);
            }
            s[offset-1] = '\n';
            writeNextResLine(s, offset); 	
        }

    }
};

// An aggregator that records a double value tom compute sum
class VERTEX_CLASS_NAME(Aggregator): public Aggregator<double> {
public:
    void init() {
        m_global = 0;
        m_local = 0;
    }
    void* getGlobal() {
        return &m_global;
    }
    void setGlobal(const void* p) {
        m_global = * (double *)p;
    }
    void* getLocal() {
        return &m_local;
    }
    void merge(const void* p) {
        m_global += * (double *)p;
    }
    void accumulate(const void* p) {
        m_local += * (double *)p;
    }
};

class VERTEX_CLASS_NAME(): public Vertex <struct message_value,double, struct message_value> {
public:
    void compute(MessageIterator* pmsgs) {

	struct message_value ver1,ver2,ver3;     
	unsigned long long num;     
 	printf("compute.........................\n");
	if(getSuperstep()==0){//the first superstep

		ver1=getValue();//get vertex value
 		num=getVertexId();//get vertex id
		printf("vertex id:");
		printf("%llu \n",num);
		printf("vertex size:");
		printf("%d\n",getVSize());
		for(int i=0;i<rowNum;i++)
			printf("%f ",ver1.matrixCol[i]);
		printf("\n");
		for(int j=0;j<colNum;j++)
			printf("%f ",ver1.identityCol[j]);
		printf("\n");
		printf("begin send message!!!!!!!...............................\n");
        		
		if(colNum%2==0)//the matrix has even columns
		{
			if(num%2==0)
			{	OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
				for( ; !outEdgeIterator.done();outEdgeIterator.next())
					if(outEdgeIterator.target() ==num+1 )
						sendMessageTo(outEdgeIterator.target(),getValue());
			}          
			else
			{
				OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
				for( ; !outEdgeIterator.done();outEdgeIterator.next())
					if(outEdgeIterator.target() ==num-1 )
						sendMessageTo(outEdgeIterator.target(),getValue());
			}
		}
		else//the matrix has odd columns
		{
			if(num!=colNum-1)
			{
				if(num%2==0)
				{
					OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
					for( ; !outEdgeIterator.done();outEdgeIterator.next())
						if(outEdgeIterator.target() ==num+1 )
							sendMessageTo(outEdgeIterator.target(),getValue());
				}
             	else
				{	
					OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
					for( ; !outEdgeIterator.done();outEdgeIterator.next())
						if(outEdgeIterator.target() ==num-1 )
							sendMessageTo(outEdgeIterator.target(),getValue());
				}		
			}
		}
	}

	else if(getSuperstep()==colNum*colNum)//if the number of superstep equal to the number of columns,orthogona and then voteToHalt()
	{
		ver1=getValue();//get vertex value
 		num=getVertexId();//get vertex id
		printf("vertex id:");
		printf("%llu\n",num);
		printf("the step:");
		printf("%d\n",getSuperstep());
		printf("vertex value:");
		for(int i=0;i<rowNum;i++)
			printf("%f ",ver1.matrixCol[i]);
		printf("\n");
		for(int j=0;j<colNum;j++)
			printf("%f ",ver1.identityCol[j]);
		printf("\n"); 
		//print recived the message

		 for ( ; ! pmsgs->done(); pmsgs->next() ) {
              	ver2=pmsgs->getValue();
				printf("recive message:");
				for(int i=0;i<rowNum;i++)
					printf("%f ",ver2.matrixCol[i]);
				printf("\n");
				for(int j=0;j<colNum;j++)
					printf("%f ",ver2.identityCol[j]);
				printf("\n"); 
       			orthogonal(ver1,ver2,rowNum,colNum);
        
        		array_assign(ver2.matrixCol,mutableValue()->matrixCol, rowNum);
      			array_assign(ver2.identityCol,mutableValue()->identityCol, colNum);
         }
         
			
		ver3=getValue();
		printf("new vertex value:");
		for(int i=0;i<rowNum;i++)
			printf("%f ",ver3.matrixCol[i]);
		printf("\n");
		for(int j=0;j<colNum;j++)
			printf("%f ",ver3.identityCol[j]);
		printf("\n"); 
		voteToHalt();
	}
	
    else if(getSuperstep()%2==1)//for the odd superstep
	{ 
		
		ver1=getValue();//get vertex value
 		num=getVertexId();//get vertex id
		printf("vertex id:");
		printf("%llu\n",num);
		printf("the step:");
		printf("%d\n",getSuperstep());
		printf("vertex value:");
		for(int i=0;i<rowNum;i++)
			printf("%f ",ver1.matrixCol[i]);
		printf("\n");
		for(int j=0;j<colNum;j++)
			printf("%f ",ver1.identityCol[j]);
		printf("\n"); 
		//print recived the message

		 for ( ; ! pmsgs->done(); pmsgs->next() ) {
                 ver2=pmsgs->getValue();
				printf("recive message:");
				for(int i=0;i<rowNum;i++)
					printf("%f ",ver2.matrixCol[i]);
				printf("\n");
				for(int j=0;j<colNum;j++)
					printf("%f ",ver2.identityCol[j]);
				printf("\n"); 
       			orthogonal(ver1,ver2,rowNum,colNum);
        
        		array_assign(ver2.matrixCol,mutableValue()->matrixCol, rowNum);
      			array_assign(ver2.identityCol,mutableValue()->identityCol, colNum);
         }
         
			
		ver3=getValue();
		printf("new vertex value:");
		for(int i=0;i<rowNum;i++)
			printf("%f ",ver3.matrixCol[i]);
		printf("\n");
		for(int j=0;j<colNum;j++)
			printf("%f ",ver3.identityCol[j]);
		printf("\n"); 

		if(colNum%2==0)//if the matrix have even columns
		{
			if((num!=0)&&(num!=colNum-1))
			{	if(num%2==1)
				{	
					OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
					for( ; !outEdgeIterator.done();outEdgeIterator.next())
						if(outEdgeIterator.target() ==num+1 )
						{
							sendMessageTo(outEdgeIterator.target(),getValue());
							printf("send to vertex id:");
							printf("%llu\n",num+1);
						}
				}          
				else
				{
					OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
					for( ; !outEdgeIterator.done();outEdgeIterator.next())
						if(outEdgeIterator.target() ==num-1 )
						{
							sendMessageTo(outEdgeIterator.target(),getValue());
							printf("send to vertex id:");
							printf("%llu\n",num-1);
						}
				}
			}
		}
		else//if the matrix have odd columns
		{
			if(0!=num)
			{
				if(num%2==0)
				{
					OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
					for( ; !outEdgeIterator.done();outEdgeIterator.next())
						if(outEdgeIterator.target() ==num-1 )
							sendMessageTo(outEdgeIterator.target(),getValue());
				}
             	else
				{	
					OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
					for( ; !outEdgeIterator.done();outEdgeIterator.next())
						if(outEdgeIterator.target() ==num+1 )
							sendMessageTo(outEdgeIterator.target(),getValue());
				}		
			}
		}		
	}
	
	else if(getSuperstep()%2==0)//for the even superstep
	{ 	
		
		ver1=getValue();//get vertex value
 		num=getVertexId();//get vertex id
		printf("vertex id:");
		printf("%llu\n",num);
		printf("the step:");
		printf("%d\n",getSuperstep());
		printf("vertex value:");
		for(int i=0;i<rowNum;i++)
			printf("%f ",ver1.matrixCol[i]);
		printf("\n");
		for(int j=0;j<colNum;j++)
			printf("%f ",ver1.identityCol[j]);
		printf("\n"); 
		//print recived the message

		 for ( ; ! pmsgs->done(); pmsgs->next() ) {
                 ver2=pmsgs->getValue();
				printf("recive message:");
				for(int i=0;i<rowNum;i++)
					printf("%f ",ver2.matrixCol[i]);
				printf("\n");
				for(int j=0;j<colNum;j++)
					printf("%f ",ver2.identityCol[j]);
				printf("\n");
       			orthogonal(ver1,ver2,rowNum,colNum);
        
        		array_assign(ver2.matrixCol,mutableValue()->matrixCol, rowNum);
      			array_assign(ver2.identityCol,mutableValue()->identityCol, colNum);
         }
         
			
		ver3=getValue();
		printf("new vertex value:");
		for(int i=0;i<rowNum;i++)
			printf("%f ",ver3.matrixCol[i]);
		printf("\n");
		for(int j=0;j<colNum;j++)
			printf("%f ",ver3.identityCol[j]);
		printf("\n"); 
		if(colNum%2==0)//if the matrix have even columns
		{
			
			if(num%2==0)
			{	
				OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
				for( ; !outEdgeIterator.done();outEdgeIterator.next())
					if(outEdgeIterator.target() ==num+1 )
					{
						sendMessageTo(outEdgeIterator.target(),getValue());
						printf("send to vertex id:");
						printf("%llu\n",num+1);
					}
			}          
			else
			{
				OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
				for( ; !outEdgeIterator.done();outEdgeIterator.next())
					if(outEdgeIterator.target() ==num-1 )
					{
						sendMessageTo(outEdgeIterator.target(),getValue());
						printf("send to vertex id:");
						printf("%llu\n",num-1);
					}
			}
			
		}
		else//if the matrix have odd columns
		{
			if((colNum-1)!=num)
			{
				if(num%2==0)
				{
					OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
					for( ; !outEdgeIterator.done();outEdgeIterator.next())
						if(outEdgeIterator.target() ==num+1 )
							sendMessageTo(outEdgeIterator.target(),getValue());
				}
             	else
				{	
					OutEdgeIterator outEdgeIterator = getOutEdgeIterator();
					for( ; !outEdgeIterator.done();outEdgeIterator.next())
						if(outEdgeIterator.target() ==num-1 )
							sendMessageTo(outEdgeIterator.target(),getValue());
				}		
			}
		}	
	}
	
}
};

class VERTEX_CLASS_NAME(Graph): public Graph {
public:
    VERTEX_CLASS_NAME(Aggregator)* aggregator;
public:
    // argv[0]: PageRankVertex.so
    // argv[1]: <input path>
    // argv[2]: <output path>
    void init(int argc, char* argv[]) {
        printf("Excute init()  at class KCoreGraph\n");
        setNumHosts(5);
        setHost(0, "localhost", 1411);
        setHost(1, "localhost", 1421);
        setHost(2, "localhost", 1431);
        setHost(3, "localhost", 1441);
        setHost(4, "localhost", 1451);

        if (argc < 5) {  
           printf ("Usage: %s <input path> <output path>\n", argv[0]);
           exit(1);
        }

        m_pin_path = argv[1];
        m_pout_path = argv[2];
        
        
        sscanf(argv[3],"%llu",&rowNum);
        sscanf(argv[4],"%llu",&colNum);
	
        //printf("K=%d",K);
        
        //aggregator = new VERTEX_CLASS_NAME(Aggregator)[1];
        //regNumAggr(1);
        //regAggr(0, &aggregator[0]);
    }
    void term() {
        delete[] aggregator;
    }
};

/* STOP: do not change the code below. */
extern "C" Graph* create_graph() {
    Graph* pgraph = new VERTEX_CLASS_NAME(Graph);
    
    pgraph->m_pin_formatter = new VERTEX_CLASS_NAME(InputFormatter);
    pgraph->m_pout_formatter = new VERTEX_CLASS_NAME(OutputFormatter);
    pgraph->m_pver_base = new VERTEX_CLASS_NAME();
    return pgraph;
}

extern "C" void destroy_graph(Graph* pobject) {
    delete ( VERTEX_CLASS_NAME()* )(pobject->m_pver_base);
    delete ( VERTEX_CLASS_NAME(OutputFormatter)* )(pobject->m_pout_formatter);
    delete ( VERTEX_CLASS_NAME(InputFormatter)* )(pobject->m_pin_formatter);
    delete ( VERTEX_CLASS_NAME(Graph)* )pobject;
}
