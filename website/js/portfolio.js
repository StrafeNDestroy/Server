// portfolio.js

// Smooth scrolling for navigation links
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
      e.preventDefault();
    
      document.querySelector(this.getAttribute('href')).scrollIntoView({
        behavior: 'smooth'
      });
    });
  });
  
  // Function to toggle active class on navigation links
  function toggleActiveClass() {
    const sections = document.querySelectorAll('section');
    
    sections.forEach(section => {
      const navLink = document.querySelector(`a[href="#${section.id}"]`);
      
      if (isElementInViewport(section)) {
        navLink.classList.add('active');
      } else {
        navLink.classList.remove('active');
      }
    });
  }
  
  // Check if an element is in the viewport
  function isElementInViewport(element) {
    const rect = element.getBoundingClientRect();
    return (
      rect.top >= 0 &&
      rect.left >= 0 &&
      rect.bottom <= (window.innerHeight || document.documentElement.clientHeight) &&
      rect.right <= (window.innerWidth || document.documentElement.clientWidth)
    );
  }
  
  // Add event listener to window scroll event
  window.addEventListener('scroll', toggleActiveClass);
  