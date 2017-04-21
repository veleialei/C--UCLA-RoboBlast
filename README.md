# COMPONENTS

## Pagination

Pagination links indicate a series of related content exists across multiple pages. Typically these are used where a multi-page approach to long lists of content improves general performance, such as in search results or inboxes.

### Contents

- Example 1
- Example 2

We use a large block of connected links for our pagination, making links hard to miss and easily scalable—all while providing large hit areas. Pagination is built with list HTML elements so screen readers can announce the number of available links. Use a wrapping `<nav>` element to identify it as a navigation section to screen readers and other assistive technologies.

In addition, as pages likely have more than one such navigation section, it’s advisable to provide a descriptive aria-label for the `<nav>` to reflect its purpose. For example, if the pagination component is used to go to next page, an appropriate label could be `aria-label="Next"`.


````
<div class="container">
    <div>
        <p>Example 1</p>
        <nav>
            <ul class="pagination">
                <li class="pagination__item">
                    <a class="pagination__link" href="#" aria-label="Previous">
                        <i class="fa fa-angle-left" aria-hidden="true"></i>
                    </a>
                </li>
                <li class="pagination__item">
                    <a class="pagination__link" href="#" aria-label="Next">
                        <i class="fa fa-angle-right" aria-hidden="true"></i>
                    </a>
                </li>
            </ul>
        </nav>
    </div>
</div>
````

