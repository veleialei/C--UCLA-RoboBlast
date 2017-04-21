# COMPONENTS

## Pagination

Pagination links indicate a series of related content exists across multiple pages. Typically these are used where a multi-page approach to long lists of content improves general performance, such as in search results or inboxes.

We use a large block of connected links for our pagination, making links hard to miss and easily scalable—all while providing large hit areas. Pagination is built with list HTML elements so screen readers can announce the number of available links. Use a wrapping `<nav>` element to identify it as a navigation section to screen readers and other assistive technologies.

### Contents

- Example 1
- Example 2

#### Example 1

If the pagination component is used to go to next page, an appropriate label could be `aria-label="Next"`.

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

#### Example 2

````
<div class="container">
    <div>
        <p>Example 2</p>
        <nav>
            <ul class="pagination">
                <li class="pagination__item">
                    <a class="pagination__link" href="#" aria-label="Previous">
                        <i class="fa fa-angle-left" aria-hidden="true"></i>
                    </a>
                </li>
                <li class="pagination__item">
                    <a class="pagination__link" href="#">1</a>
                </li>
                <li class="pagination__item">
                    <a class="pagination__link" href="#">2</a>
                </li>
                <li class="pagination__item">
                    <a class="pagination__link" href="#">3</a>
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
