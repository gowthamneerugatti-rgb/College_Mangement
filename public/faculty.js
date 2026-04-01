async function loadFaculty() {
    try {
        const response = await fetch('/api/faculty/all');
        const data = await response.json();
        const tbody = document.querySelector('#facultyTable tbody');
        tbody.innerHTML = '';
        data.forEach(fac => {
            tbody.innerHTML += `
                <tr>
                    <td>${fac.id}</td>
                    <td>${fac.name}</td>
                    <td>${fac.department}</td>
                    <td>${fac.designation}</td>
                    <td>₹${fac.salary}</td>
                    <td><button onclick="deleteFaculty(${fac.id})">Delete</button></td>
                </tr>
            `;
        });
    } catch (e) {
        console.error('Failed to load faculty:', e);
        throw e;
    }
}

document.getElementById('addFacultyForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const faculty = {
        id: Number.parseInt(document.getElementById('facId').value),
        name: document.getElementById('facName').value,
        department: document.getElementById('facDept').value,
        designation: document.getElementById('facDesignation').value,
        salary: Number.parseFloat(document.getElementById('facSalary').value)
    };

    await fetch('/api/faculty/add', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(faculty)
    });
    document.getElementById('addFacultyForm').reset();
    loadFaculty();
});

async function deleteFaculty(id) {
    await fetch(`/api/faculty/delete?id=${id}`, { method: 'DELETE' });
    loadFaculty();
}

await loadFaculty();
