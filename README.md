# CS-330-Comp-Graphic-and-Visualization

# 3D Objects in OpenGL

## Project Overview

This project focuses on creating and manipulating 3D objects using OpenGL. It demonstrates the basic concepts of 3D rendering, transformations (translation, rotation, scaling), and camera management to create an interactive 3D scene. Through this project, I’ve learned to use OpenGL’s rendering pipeline to handle basic 3D objects, implement transformations, and add camera control.

## Table of Contents
1. [Installation](#installation)
2. [Technologies Used](#technologies-used)
3. [Features](#features)
4. [Development Process](#development-process)
5. [Usage](#usage)
6. [Contributing](#contributing)
7. [License](#license)

## Installation

To get the project up and running locally, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/nwyrwas/CS-330-Comp-Graphic-and-Visualization
   ```

   <br>
2. Navigate to the project directory:
  ```bash
    cd 3d-objects-opengl
  ``` 
<br>

3. Make sure you have the required libraries installed:

OpenGL: The main library for rendering 3D graphics.
GLFW: A library for creating windows and handling input.
GLAD: OpenGL loading library for handling extensions.
<br>

4. Install GLFW and GLAD via your package manager or from their respective websites.
   <br>



<div class="section" id="technologies-used">
    <h2><u><b>Technologies Used</b></u></h2>
    <ul>
        <li><strong>OpenGL</strong>: Used for rendering 3D graphics.</li>
        <li><strong>GLFW</strong>: Library for creating windows, handling input, and managing OpenGL contexts.</li>
        <li><strong>GLAD</strong>: Used to load OpenGL extensions.</li>
        <li><strong>GLM</strong>: A library for mathematics and transformations in 3D graphics.</li>
        <li><strong>C++</strong>: Programming language used for implementing the project.</li>
    </ul>
</div>


<div class="section" id="features">
    <h2><u><b>Features</b></u></h2>
    <ul>
        <li><strong>3D Object Rendering</strong>: Renders basic shapes (cubes, spheres, etc.).</li>
        <li><strong>Transformations</strong>: Supports translation, rotation, and scaling of objects in 3D space.</li>
        <li><strong>Camera Controls</strong>: Allows users to manipulate the camera to view objects from different angles.</li>
        <li><strong>Lighting</strong>: Implements basic lighting models to give depth and realism to 3D objects.</li>
    </ul>
</div>

<div class="section" id="development-process">
    <h2 style="text-align:center;"><u><b>Development Process</b></u></h2>
</div>
    
<h3>Development Process</h3>
<h3><u><b>How Do I Approach Designing Software?</b></u></h3>
<p>When designing software, I focus on a systematic approach to break down large problems into smaller, manageable components. I begin by setting up the foundational structure, such as initializing the OpenGL context and rendering a basic 3D object, like a cube or sphere. This allows me to ensure that the basic rendering framework works before adding complexity. From here, I incrementally add features like 3D transformations, camera controls, and lighting effects, testing each one independently to ensure stability.</p>
<p>Each feature is modular, meaning that it can be tested and refined individually before integrating it into the larger system. This approach not only facilitates debugging but also makes it easier to extend the system in the future. I also prioritize writing clean, well-documented code to enhance the maintainability of the software and ensure that future developers can easily navigate and modify the codebase. Through this approach, I ensure that I’m building software that is both scalable and reliable.</p>

<h3><u><b>What New Design Skills Has Your Work on the Project Helped You Craft?</b></u></h3>
<p>Throughout this project, I have significantly enhanced my ability to work with graphical systems and develop interactive 3D applications. One of the key skills I developed is managing 3D transformations, which involves applying operations like translation, scaling, and rotation to objects within a 3D space. I’ve also strengthened my understanding of coordinate systems and spatial relationships, which are fundamental for accurate rendering and camera control.</p>
<p>In addition, I’ve gained hands-on experience with real-time performance optimization. For instance, I learned how to efficiently manage memory and resources to ensure smooth frame rates during complex rendering operations. This has also led to a deeper understanding of shaders, lighting, and materials, which are essential for creating visually appealing and realistic scenes. These skills are invaluable for any future work in game development, simulations, or any field involving 3D graphics.</p>

<h3><u><b>What Design Process Did You Follow for Your Project Work?</b></u></h3>
<p>The design process for this project was iterative, where I started with a simple framework and progressively added features. I first set up the OpenGL context and rendered a basic 3D object to verify that the rendering pipeline was working. Once the basics were in place, I focused on adding 3D transformations, allowing me to manipulate objects in space by applying translation, rotation, and scaling. This was followed by adding camera controls, which were crucial for user interaction and navigation through the scene.</p>
<p>With the core features functioning, I began working on improving the visual quality by introducing lighting models. This included implementing ambient, directional, and point light sources to simulate different lighting conditions. Throughout the process, I continually tested and refined the system. This iterative approach allowed me to catch and resolve bugs early, ensuring that each component worked as expected before moving on to the next one.</p>

<h3><u><b>How Could Tactics from Your Design Approach Be Applied in Future Work?</b></u></h3>
<p>The strategies I applied in this project—breaking down large tasks into smaller, more manageable components and using an iterative approach—can be applied to future software development projects, especially those involving complex systems. By focusing on modularity, each part of the system can be developed, tested, and improved independently, leading to more robust and maintainable software. This approach also makes it easier to isolate and address bugs, as smaller components are easier to debug and test in isolation.</p>
<p>Additionally, the iterative approach promotes continuous improvement. In future projects, especially those requiring real-time performance (such as simulations or game development), I will apply this strategy to ensure that new features are added progressively while maintaining the stability and efficiency of the system. This method of development reduces the risk of overwhelming the system with large, untested changes, ultimately leading to a smoother development process and higher-quality software.</p>




<div class="section" id="usage">
    <h2 style="text-align:center;"><u><b>Usage</b></u></h2>
    <p>Once you have the program set up, running it will open a window displaying a 3D scene with basic objects. You can interact with the scene using the keyboard and mouse:</p>
    <ul>
        <li><b>W, A, S, D:</b> Move the camera forward, left, backward, and right.</li>
        <li><b>Arrow keys:</b> Rotate the camera to look around.</li>
        <li><b>Mouse:</b> Click and drag to change the view.</li>
    </ul>
</div>

<div class="section" id="contributing">
    <h2 style="text-align:center;"><u><b>Contributing</b></u></h2>
    <p>Contributions are welcome! If you have suggestions or improvements, feel free to open an issue or submit a pull request. Please ensure your changes follow the existing code style and pass any provided tests.</p>
</div>

<div class="section" id="license">
    <h2 style="text-align:center;"><u><b>License</b></u></h2>
    <p>This project is licensed under the MIT License - see the LICENSE file for details.</p>
</div>

<div class="section" id="development-process">
    <h2 style="text-align:center;"><u><b>Development Process</b></u></h2>
</div>
<ul>
  <li><b>How Do I Approach Developing Programs?</b>
    <p>When developing programs, I aim to write clear, maintainable, and efficient code that addresses the problem at hand incrementally. I start by analyzing the requirements and breaking the problem into smaller, manageable components. This ensures that I can focus on one part at a time while maintaining a clear understanding of how everything fits together. I ensure that the code I write is modular and reusable by creating well-defined functions and classes, which allows for easier maintenance and future updates. Each module is designed with a single responsibility in mind, making the program easier to understand and extend.</p>
    <p>Another key aspect of my development process is prioritizing debugging and testing from the very beginning. I write unit tests alongside my code to catch potential issues early. Testing frequently helps identify bugs before they become more difficult to fix later in the process. This proactive approach ensures that the program remains stable as I add new features. Additionally, I focus on code readability by following consistent naming conventions and adding comments, which makes the codebase more understandable for others (and myself) in the future.</p>
  </li>

  <li><b>What New Development Strategies Did You Use While Working on Your 3D Scene?</b>
    <p>While developing the 3D scene, I adopted an iterative development approach, where I focused on building the system step by step, testing and refining each feature along the way. I began by setting up the basic OpenGL context and rendering simple shapes such as cubes and spheres. This allowed me to confirm that the framework was working as expected before moving on to more complex tasks.</p>
    <p>Once the basics were in place, I progressively added more sophisticated features, such as transformations (translation, scaling, and rotation) and camera controls, to make the scene interactive. I implemented real-time camera movement to navigate the 3D space and adjusted the scene dynamically based on user input. Afterward, I added lighting to enhance the visual quality of the scene, testing the effects of different light sources and ensuring proper interaction between objects and lighting. Throughout this process, I paid close attention to performance, continuously optimizing the code to ensure smooth, real-time rendering.</p>
  </li>

  <li><b>How Did Iteration Factor into Your Development?</b>
    <p>Iteration played a critical role in the development of the 3D scene. By starting with basic functionality, I was able to build a working foundation that could be expanded incrementally. After implementing each feature, I tested it thoroughly to ensure that it functioned as expected. This continuous testing and refinement cycle allowed me to catch bugs early and make improvements on the go.</p>
    <p>The iterative process also allowed me to gradually add complexity while keeping the system manageable. For example, I began by implementing simple transformations, then tested them before moving on to more advanced features like camera movement and lighting. This approach helped me focus on small, isolated components and ensured that I didn’t become overwhelmed by the complexity of the project as a whole. Additionally, iterative development provided an opportunity to revisit and optimize earlier features based on the feedback from new additions, resulting in a more efficient final product.</p>
  </li>

  <li><b>How Has Your Approach to Developing Code Evolved Throughout the Milestones, Leading to the Project’s Completion?</b>
    <p>My approach to developing code has evolved significantly as I worked through the various milestones of the project. Initially, I was focused on understanding how to set up OpenGL and render basic shapes. This early stage was crucial for learning how to manage graphics pipelines and get comfortable with the development environment. As I moved forward, my focus shifted towards learning more advanced topics like transformations and handling real-time user input.</p>
    <p>As the project progressed, I also placed a stronger emphasis on performance optimization. Early on, rendering was simple, but as I added more features like camera controls and lighting, I had to ensure that the system could handle complex operations without dropping frame rates. My understanding of graphics programming deepened significantly, and I began to make decisions about memory management and optimization techniques to improve efficiency. By the end of the project, I had developed a solid understanding of how to balance visual complexity with real-time rendering performance, which will be beneficial for any future graphical software projects.</p>
  </li>

  <li><b>How Can Computer Science Help Me in Reaching My Goals?</b>
    <p>Computer science provides the foundational knowledge and problem-solving techniques necessary to address a wide range of challenges in software development. By applying principles like algorithms, data structures, and software engineering practices, I can approach complex problems with a systematic mindset. This enables me to break down large problems into smaller, more manageable tasks, ensuring that I can tackle even the most difficult issues efficiently.</p>
    <p>In my case, computer science has specifically helped me pursue my goals in areas such as graphics programming, real-time simulations, and interactive systems. With a strong foundation in core computer science concepts, I can approach projects like 3D rendering or simulation software with confidence, knowing that I can apply logical thinking and optimized algorithms to create efficient solutions. Furthermore, the ability to think critically and logically is essential for future work in areas like AI, game development, and augmented reality, which require complex problem-solving skills.</p>
  </li>

  <li><b>How Do Computational Graphics and Visualizations Give You New Knowledge and Skills That Can Be Applied in Your Future Educational Pathway?</b>
    <p>Working with computational graphics has provided me with invaluable insights into how mathematical concepts like matrices, vectors, and geometric transformations are applied to real-world problems. For example, I’ve gained a deeper understanding of how matrices are used to perform transformations on objects within a 3D space and how vectors help to define the direction and position of those objects.</p>
    <p>These concepts are crucial not just for graphics programming but also for other fields of computer science, such as simulations, robotics, and AI. Computational graphics has also taught me how to think in terms of space and motion, which will be beneficial in courses that involve simulations or physical modeling. As I continue my studies, I can apply these techniques to more advanced topics like computational geometry, computer-aided design (CAD), or even virtual reality systems. The skills I’m building in computational graphics will provide a solid foundation for tackling these advanced subjects in future educational pathways.</p>
  </li>

  <li><b>How Do Computational Graphics and Visualizations Give You New Knowledge and Skills That Can Be Applied in Your Future Professional Pathway?</b>
    <p>Computational graphics and visualizations are increasingly important in a variety of professional fields, such as game development, augmented reality, simulation, and visual effects. The ability to efficiently render 3D objects and manage complex visual scenes is a critical skill in these industries. As I continue to develop my expertise in computational graphics, I’ll be better prepared for roles in which I can create high-quality visual experiences and interactive applications.</p>
    <p>In particular, skills in computational graphics will be invaluable in the growing fields of game development and augmented reality. These industries rely heavily on real-time rendering and dynamic visualizations, and having the expertise to develop such systems will give me a competitive edge in the job market. Additionally, computational graphics is essential in simulation environments, such as virtual training or medical simulations, where realistic visualizations are crucial. With this expertise, I’ll be able to contribute to developing innovative technologies that require efficient graphics processing and realistic visual representation.</p>
  </li>
</ul>






