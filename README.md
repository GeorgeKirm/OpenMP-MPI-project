
<h1>OpenMP/OpenMPI/Cuda project</h1>

<h2>What does this project:</h2>
<ul>
	<li>This project generates a file with a huge list of the 3 vallues of the location of many points in space,
	and use the datafile that was generated for 2 other programs which do check if the points have a certain distance from zero.
	The first is a Seriac version and the second is the Parallel version of the program.
	The point of the program is to compare the execution time,how many points had bigger distance etc. 
	Generaly the differences between both excecutions</li>
</ul>
<h3>This project was made in order we learn a bit of:</h3>
<ul>
	<li>OpenMP</li>
	<li>OpenMPI</li>
	<li>and Cuda</li>
</ul>
<h3>Both branches are part of the project</h3>
<ul>
	<li>The first is about the OpenMp and OpenMPI time and value usage compared to a serial version</li>
		<h4>Branch "master" contains</h4>
		<ol>
			<li>A program which Generates a datafile acording to makefile's variables.</li>
			<li>A program which is the Siriac version and which will use the datafile.</li>
			<li>A program which is the Parallel version and which will use the datafile.</li>
		</ol>
</ul>
<ul>
	<li>Branch "Cuda" is about Cuda time and value usage compared to a serial</li>
		<h4>Branch "Cuda" contains</h4>
		<ol>
			<li>A program which Generates a datafile acording to makefile's variables.</li>
			<li>A program which is the Siriac version and which will use the datafile and does not use functions that Parralel version can not use.</li>
			<li>A program which is the Parallel version on Cuda and which will use the datafile</li>
		</ol>
</ul>
